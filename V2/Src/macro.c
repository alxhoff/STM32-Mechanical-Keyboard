/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include "error.h"
#include "send.h"

#include <stdlib.h>
#include <string.h>

typedef struct macro_entry macro_entry_t;

struct macro_entry {
	unsigned char sc;

	char* string;

	macro_entry_t *next;
};

typedef struct macro_table {
	macro_entry_t *head;
	macro_entry_t *tail;

	int count;
} macro_table_t;

macro_table_t macro_dev = { 0 };

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

unsigned char macro_add_new_entry(char *string, unsigned char sc) {
	macro_entry_t *macro = calloc(1, sizeof(macro_entry_t));
	if(!macro)
		return -ENOMEM;

	macro->string = malloc(sizeof(char) * (strlen(string) + 1));
	if (!macro->string)
		goto string_error;

	strcpy(macro->string, string);
	macro->sc = sc;

	if (!macro_dev.head) {
		macro_dev.head = macro;
		macro_dev.tail = macro;
	} else {
		macro_dev.tail->next = macro;
		macro_dev.tail = macro;
	}

	return 0;

	string_error: free(macro);
	return -ENOMEM;
}

//TODO check this
unsigned char macro_rem_entry(unsigned char sc) {
	macro_entry_t *head = macro_dev.head;
	if (head->sc == sc) {
		//single item list
		if (!head->next) {
			macro_dev.head = NULL;
			macro_dev.tail = NULL;
		} else
			//longer than one item
			macro_dev.head = head->next;
		free(head);
		return 0;
	}

	test:	//if list has more than one item
	if (head->next->sc == sc) {
		if (!head->next->next) 	//is tail
			macro_dev.tail = head;
		free(head->next);
		return 0;
	}

	while (head->next->next) {
		head = head->next;
		goto test;
	}
	return -ENOENT;
}

macro_entry_t* macro_get_sc(unsigned char sc) {
	macro_entry_t *head = macro_dev.head;

	test: if (head->sc == sc)
		return head;

	while (head->next) {
		head = head->next;
		goto test;
	}

	return NULL;
}

unsigned char macro_run_sc(unsigned char sc) {
	macro_entry_t *macro = macro_get_sc(sc);
	if (!macro)
		return -ENOENT;

	return send_string(macro->string);
}
