/*
 * states.c
 *
 *  Created on: Sep 10, 2017
 *      Author: alxhoff
 */

#include "states.h"
#include "keymap.h"

keyboard_states current_keyboard_state;

states_err_typeDef state_enter_layer_set()
{
	current_keyboard_state = layer_set;
	vTaskDelay(100);
	return states_ok;
}

states_err_typeDef state_exit_layer_set()
{
	current_keyboard_state = typing;
}

states_err_typeDef state_layer_set( keymap_list* layer_list )
{
	key_code layer = (scan_get_single_key( layer_list ) - 0x1E);
	//TODO CHECKL THAT RET IS VALID LAYER
	layer_list->current_layer = layer;
	state_exit_layer_set();
}
