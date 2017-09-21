/*
 * macro.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef MACRO_H_
#define MACRO_H_

#include "keymap.h"
#include "keyboard.h"
#include "types.h"


typedef struct macro_entry macro_entry_t;
typedef struct keymap_list keymap_list_t;

struct macro_entry{
	uint8_t key_code;

	char* keypress_string;

	macro_entry_t* next;
};

typedef struct macro_table{
	macro_entry_t* head;

	uint8_t count;
} macro_table_t;

states_err_t macro_init( key_devices_t* key_devs );
states_err_t macro_table_add_entry( macro_table_t* table, macro_entry_t* entry );
macro_entry_t* macro_table_get_last( macro_table_t* table );
macro_entry_t* macro_table_get_w_key_code( macro_table_t* table, key_code key );
states_err_t macro_execute_macro( macro_table_t* table, macro_entry_t* macro );
states_err_t macro_send_blank( keyboardHID_t* macro_report );
macro_entry_t* macro_allocate_new_macro( macro_table_t* table );
#endif /* MACRO_H_ */
