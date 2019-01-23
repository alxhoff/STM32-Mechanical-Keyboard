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
	uint8_t sc;

	char* string;

	macro_entry_t* next;
};

typedef struct macro_table{
	macro_entry_t* head;

	uint8_t count;
} macro_table_t;

int8_t macro_init( key_devices_t* key_devs );
int8_t macro_table_add_entry( macro_table_t* table, macro_entry_t* entry );
int8_t state_enter_macro_set();
int8_t state_exit_macro_set();
int8_t state_macro_set( keymap_list_t* layer_list );
int8_t state_enter_macro_run();
int8_t state_exit_macro_run();
int8_t state_macro_run( keymap_list_t* layer_list );
macro_entry_t* macro_allocate_new_macro( macro_table_t* table );
int8_t macro_send_blank( keyboardHID_t* macro_report );
int8_t macro_execute_macro( macro_table_t* table, macro_entry_t* macro );
macro_entry_t* macro_table_get_w_key_code( macro_table_t* table, key_code key );
macro_entry_t* macro_table_get_last( macro_table_t* table );


#endif /* MACRO_H_ */
