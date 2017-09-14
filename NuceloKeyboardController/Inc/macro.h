/*
 * macro.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef MACRO_H_
#define MACRO_H_

#include "states.h"
#include "keymap.h"

typedef struct macro_entry macro_entry_t;

struct macro_entry{
	uint8_t key_code;

	char* keypress_string;

	macro_entry_t* next;
};

typedef struct macro_table{
	macro_entry_t* head;

	uint8_t count;
} macro_table_t;

states_err_t macro_init( keymap_list_t* list );
states_err_t macro_table_add_entry( keymap_list_t* list, macro_entry_t* entry );
macro_entry_t* macro_table_get_last( macro_table_t* table );
macro_entry_t* macro_table_get_w_key_code( keymap_list_t* list, key_code key );
states_err_t macro_execute_macro( keymap_list_t* list, macro_entry_t* macro );
states_err_t macro_send_blank( keyboardHID_t* macro_report );

#endif /* MACRO_H_ */
