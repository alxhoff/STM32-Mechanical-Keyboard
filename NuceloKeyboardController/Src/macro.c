/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include <stdio.h>

#include "macro.h"
#include "lookup.h"
#include "extern.h"

states_err_t state_enter_macro_set()
{
	current_keyboard_state = macro_set;
	vTaskDelay(100);
	return states_ok;
}

states_err_t state_exit_macro_set()
{
	current_keyboard_state = typing;
	return states_ok;
}

states_err_t state_macro_set( keymap_list_t* layer_list )
{
	key_code macro_key = scan_get_single_key( layer_list );
	//TODO LIGHTS
	macro_entry_t* new_macro = macro_allocate_new_macro( layer_list );
	new_macro->key_code = macro_key; //GOOD
	new_macro->keypress_string = scan_get_input_seq( layer_list );

	state_exit_macro_set();
	return states_ok;
}

states_err_t state_enter_macro_run()
{
	current_keyboard_state = macro_run;
	vTaskDelay(100);

	return states_ok;
}

states_err_t state_exit_macro_run()
{
	current_keyboard_state = typing;
	return states_ok;
}

states_err_t state_macro_run( keymap_list_t* layer_list )
{
	key_code macro_key = scan_get_single_key( layer_list );
	static macro_entry_t* cur_macro;
	cur_macro = macro_table_get_w_key_code( layer_list, macro_key );
	macro_execute_macro(layer_list,cur_macro);
	state_exit_macro_run();

	return states_ok;
}

states_err_t macro_init( key_devices_t* key_devs )
{
	key_devs->macro_table = (macro_table_t*) malloc(sizeof(macro_table_t));
	if(key_devs->macro_table == NULL)
		return states_init_err;

	key_devs->macro_table->count = 0;
	key_devs->macro_table->head = NULL;
	return states_init_ok;
}

states_err_t macro_table_add_entry( macro_table_t* table, macro_entry_t* entry )
{
	if(table->count == 0){
		table->head = entry;
	}else{
		macro_entry_t* last = macro_table_get_last(table);
		last->next = entry;
	}

	table->count++;

	return states_ok;
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

	while(head->key_code != key){
		head = head->next;
		if(head == NULL)
			return NULL;
	}

	return head;
}

states_err_t macro_execute_macro( macro_table_t* table, macro_entry_t* macro )
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
	while(macro->keypress_string[i] != '\0'){
		if(i >=1 && macro->keypress_string[i] == macro->keypress_string[i-1]){
//			macro_report.key1 = 0;
			macro_send_blank( &macro_report );
			USBD_HID_SendReport(&hUsbDeviceFS, &macro_report, sizeof(keyboardHID_t));
			vTaskDelay(16);
		}
		macro_report.key1 =	lookup_sc[(uint8_t)macro->keypress_string[i]].scanCode;
		macro_report.modifiers = lookup_sc[(uint8_t)macro->keypress_string[i]].modifier;
		USBD_HID_SendReport(&hUsbDeviceFS, &macro_report, sizeof(keyboardHID_t));

		vTaskDelay(16);
		i++;
	}
	macro_send_blank( &macro_report );

	return states_ok;
}

states_err_t macro_send_blank( keyboardHID_t* macro_report )
{
	uint8_t* reset = &macro_report->key1;
	for(uint8_t i =0; i< 6; i++){
		*reset = 0;
		reset += sizeof(macro_report->key1);
	}
	macro_report->modifiers = 0;
	USBD_HID_SendReport(&hUsbDeviceFS, macro_report, sizeof(keyboardHID_t));

	return states_ok;
}

macro_entry_t* macro_allocate_new_macro( macro_table_t* table )
{
	macro_entry_t* new = (macro_entry_t*) malloc(sizeof(macro_entry_t));
	if(new == NULL)
		return NULL;

	new->keypress_string = NULL;
	new->next = NULL;
	new->key_code = NULL;

	//TODO empty table check
	macro_entry_t* last = macro_table_get_last( table );

	last->next = new;
	return new;
}
