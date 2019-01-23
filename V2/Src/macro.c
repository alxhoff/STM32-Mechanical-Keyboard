/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include "error.h"
#include "macro.h"
#include "lookup.h"
#include "keyboard.h"
#include "usb_device.h"

typedef struct macro_table{
	macro_entry_t 	*head;
	macro_entry_t 	*tail;

	int 			count;
} macro_table_t;

macro_table_t macro_dev = {0};

//unsigned char state_enter_macro_set()
//{
//	current_keyboard_state = macro_set;
//	vTaskDelay(200);
//	return 0;
//}
//
//unsigned char state_exit_macro_set()
//{
//	current_keyboard_state = typing;
//	vTaskDelay(200);
//	return 0;
//}
//
//unsigned char state_macro_set( keymap_list_t* layer_list )
//{
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//
//	key_code macro_key = scan_get_single_key( keyboard_devs->keyboard, layer_list );
//	//TODO LIGHTS
//	macro_entry_t* new_macro = macro_allocate_new_macro( keyboard_devs->macro_table );
//	new_macro->sc = macro_key; //GOOD
//	new_macro->string =
//			scan_get_input_seq( keyboard_devs->keyboard,
//					keyboard_devs->layer_list, KEY(MACRO_S) );
//
//	state_exit_macro_set();
//
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//	return 0;
//}
//
//unsigned char state_enter_macro_run()
//{
//	current_keyboard_state = macro_run;
//	vTaskDelay(200);
//
//	return 0;
//}
//
//unsigned char state_exit_macro_run()
//{
//	current_keyboard_state = typing;
//	return 0;
//}
//
//unsigned char state_macro_run( keymap_list_t* layer_list )
//{
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//
//	key_code macro_key = scan_get_single_key( keyboard_devs->keyboard, layer_list );
//	macro_entry_t* cur_macro;
//	cur_macro = macro_table_get_w_key_code( keyboard_devs->macro_table, macro_key );
//	macro_execute_macro( keyboard_devs->macro_table, cur_macro );
//	state_exit_macro_run();
//
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//	return 0;
//}

unsigned char macro_add_new_entry( char *string, scan_code_t *sc )
{
	char *string_cpy = malloc(sizeof(char) * (strlen(string) + 1));
	if(!string_cpy)
		return -ENOMEM;

	macro_entry_t *macro = calloc(1, sizeof(macro_entry_t));
	if(!macro)
		goto macro_error;

	strcpy(string_cpy, string);
	memcpy(&macro->sc, sc, sizeof(scan_code_t));

	if(!macro_dev.head){
		macro_dev.head = macro;
		macro_dev.tail = macro;
	}else{
		macro_dev.tail->next = macro;
		macro_dev.tail = macro;
	}

	return 0;

macro_error:
	free(string_cpy);
	return -ENOMEM;
}

void macro_add_entry(macro_entry_t *entry)
{
	if(!macro_dev.head){
		macro_dev.head = entry;
		macro_dev.tail = entry;
	}else{
		macro_dev.tail->next = entry;
		macro_dev.tail = entry;
	}
}

//TODO check this
unsigned char macro_rem_entry(scan_code_t *sc)
{
	macro_entry_t *head = macro_dev.head;
	macro_entry_t *prev = NULL;
	if(memcmp(&head->sc, sc, sizeof(scan_code_t))){
		//single item list
		if(!head->next){
			macro_dev.head = NULL;
			macro_dev.tail = NULL;
		}else //longer than one item
			macro_dev.head = head->next;
		free(head);
		return 0;
	}

test:	//if list has more than one item
	if(memcmp(&head->next->sc, sc, sizeof(scan_code_t))){
		if(!head->next->next) 	//is tail
			macro_dev.tail = head;
		free(head->next);
		return 0;
	}

	while(head->next->next){
		head = head->next;
		goto test;
	}
	return -ENOENT;
}

macro_entry_t* macro_get_sc(scan_code_t *sc)
{
	macro_entry_t *head = macro_dev.head;

test:
	if(memcmp(&head->sc, sc, sizeof(scan_code_t)))
		return head;

	while(head->next){
		head = head->next;
		goto test;
	}

	return NULL;
}

unsigned char macro_execute_macro( macro_entry_t* macro )
{
	static keyboardHID_t macro_report = {
			.id = 1
	};

	unsigned int i = 0;

	while(macro->string[i] != '\0'){
		if(i && macro->string[i] == macro->string[i-1]){
			keyboard_send_blank();
			vTaskDelay(16);
		} //TODO move to keyboard function
		macro_report.key1 =	lookup_get_key((unsigned char)macro->string[i]);
		macro_report.modifiers = lookup_get_mod((unsigned char)macro->string[i]);
		USBD_HID_SendReport(&hUsbDeviceFS, &macro_report, sizeof(keyboardHID_t));

		vTaskDelay(16);
		i++;
	}
	macro_send_blank( &macro_report );

	return 0;
}

unsigned char macro_run_sc(scan_code_t *sc)
{
	macro_entry_t *macro = macro_get_sc(sc);
	if(!macro)
		return -ENOENT;

	return macro_execute_macro(macro);
}
