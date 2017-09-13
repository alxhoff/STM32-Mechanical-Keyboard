/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include <stdio.h>

#include "keymap.h"
#include "states.h"
#include "macro.h"

states_err_t state_enter_macro_set()
{

	return states_ok;
}

states_err_t state_exit_macro_set()
{
	current_keyboard_state = typing;
	return states_ok;
}

states_err_t state_macro_set()
{

	return states_ok;
}

states_err_t state_enter_macro_run()
{

	return states_ok;
}

states_err_t state_exit_macro_run()
{
	current_keyboard_state = typing;
	return states_ok;
}

states_err_t state_macro_run()
{

	return states_ok;
}

states_err_t macro_init( keymap_list_t* list )
{
	macro_table_t* table = (macro_table_t*) malloc(sizeof(macro_table_t));
	if(table == NULL)
		return states_init_err;
	list->macro_table = table;

	list->macro_table->count = 0;
	list->macro_table->head = NULL;
	return states_init_ok;
}

states_err_t macro_table_add_entry( keymap_list_t* list, macro_entry_t* entry )
{
	if(list->macro_table->count == 0){
		list->macro_table->head = entry;
	}else{
		macro_entry_t* last = macro_table_get_last(list->macro_table);
		last->next = entry;
	}

	list->macro_table->count++;

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
