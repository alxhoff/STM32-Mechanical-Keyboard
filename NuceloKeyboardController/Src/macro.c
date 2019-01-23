/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include <stdio.h>

#include "scan.h"
#include "macro.h"
#include "lookup.h"
#include "extern.h"

int8_t state_enter_macro_set()
{
	current_keyboard_state = macro_set;
	vTaskDelay(200);
	return 0;
}

int8_t state_exit_macro_set()
{
	current_keyboard_state = typing;
	vTaskDelay(200);
	return 0;
}

int8_t state_macro_set( keymap_list_t* layer_list )
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	key_code macro_key = scan_get_single_key( keyboard_devs->keyboard, layer_list );
	//TODO LIGHTS
	macro_entry_t* new_macro = macro_allocate_new_macro( keyboard_devs->macro_table );
	new_macro->sc = macro_key; //GOOD
	new_macro->string =
			scan_get_input_seq( keyboard_devs->keyboard,
					keyboard_devs->layer_list, KEY(MACRO_S) );

	state_exit_macro_set();

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	return 0;
}

int8_t state_enter_macro_run()
{
	current_keyboard_state = macro_run;
	vTaskDelay(200);

	return 0;
}

int8_t state_exit_macro_run()
{
	current_keyboard_state = typing;
	return 0;
}

int8_t state_macro_run( keymap_list_t* layer_list )
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	key_code macro_key = scan_get_single_key( keyboard_devs->keyboard, layer_list );
	macro_entry_t* cur_macro;
	cur_macro = macro_table_get_w_key_code( keyboard_devs->macro_table, macro_key );
	macro_execute_macro( keyboard_devs->macro_table, cur_macro );
	state_exit_macro_run();

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	return 0;
}

int8_t macro_init( key_devices_t* key_devs )
{
	key_devs->macro_table = (macro_table_t*) calloc(1, sizeof(macro_table_t));
	if(key_devs->macro_table == NULL)
		return -ENOMEM;

	return 0;
}

int8_t macro_table_add_entry( macro_table_t* table, macro_entry_t* entry )
{
	macro_entry_t* new_entry = (macro_entry_t*)calloc(1, sizeof(macro_entry_t));
	if(new_entry == NULL) return -ENOMEM;

	new_entry->sc = entry->sc;
	new_entry->string = (char*)malloc(sizeof(char) *
			(strlen(entry->string) +1 ));
	strcpy(new_entry->string, entry->string);

	if(table->count == 0){
		table->head = new_entry;
	}else{
		macro_entry_t* last = macro_table_get_last(table);
		last->next = new_entry;
	}

	table->count++;

	return 0;
}

macro_entry_t* macro_table_get_last( macro_table_t* table )
{
	if(table->count == 0)
		return NULL;

	macro_entry_t* head = table->head;

	while(head->next != NULL)
		head = head->next;

	return head;
}

macro_entry_t* macro_table_get_w_key_code( macro_table_t* table, key_code key )
{
	macro_entry_t* head = table->head;

	while(head->sc != key){
		head = head->next;
		if(head == NULL)
			return NULL;
	}

	return head;
}

int8_t macro_execute_macro( macro_table_t* table, macro_entry_t* macro )
{
	static keyboardHID_t macro_report = {
			.id = 1,
			.key1 = 0,
			.key2 = 0,
			.key3 = 0,
			.key4 = 0,
			.key5 = 0,
			.key6 = 0,
			.modifiers = 0
	};

	uint16_t i = 0;
	while(macro->string[i] != '\0'){
		if(i >=1 && macro->string[i] == macro->string[i-1]){
//			macro_report.key1 = 0;
			macro_send_blank( &macro_report );
			USBD_HID_SendReport(&hUsbDeviceFS, &macro_report, sizeof(keyboardHID_t));
			vTaskDelay(16);
		}
		macro_report.key1 =	lookup_sc[(uint8_t)macro->string[i]].code;
		macro_report.modifiers = lookup_sc[(uint8_t)macro->string[i]].mod;
		USBD_HID_SendReport(&hUsbDeviceFS, &macro_report, sizeof(keyboardHID_t));

		vTaskDelay(16);
		i++;
	}
	macro_send_blank( &macro_report );

	return 0;
}

int8_t macro_send_blank( keyboardHID_t* macro_report )
{
	uint8_t* reset = &macro_report->key1;
	for(uint8_t i =0; i< 6; i++){
		*reset = 0;
		reset += sizeof(macro_report->key1);
	}
	macro_report->modifiers = 0;
	USBD_HID_SendReport(&hUsbDeviceFS, macro_report, sizeof(keyboardHID_t));

	return 0;
}

macro_entry_t* macro_allocate_new_macro( macro_table_t* table )
{
	macro_entry_t* new = (macro_entry_t*) malloc(sizeof(macro_entry_t));
	if(new == NULL)
		return NULL;

	new->string = NULL;
	new->next = NULL;
	new->sc = NULL;

	//TODO empty table check
	macro_entry_t* last = macro_table_get_last( table );

	last->next = new;
	return new;
}
