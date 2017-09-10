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

	return states_ok;
}

states_err_typeDef state_layer_set( keymap_list* layer_list )
{
	key_code layer = scan_get_single_key( layer_list );
}
