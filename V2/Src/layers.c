/*
 * layers.c
 *
 *  Created on: Sep 10, 2017
 *      Author: alxhoff
 */

#include "layers.h"
#include "cmsis_os.h"

unsigned char state_enter_layer_set(void)
{
//	current_keyboard_state = layer_set;
	vTaskDelay(100);
//	return states_ok;
}

unsigned char state_exit_layer_set(void)
{
//	current_keyboard_state = typing;
//	return states_ok;
}

unsigned char state_layer_set(void)
{
//	key_code layer = scan_get_single_key( keyboard_devs->keyboard, layer_list );
//	//TODO CHECKL THAT RET IS VALID LAYER
//	layer_list->current_layer = layer_table_get_ID_w_key(layer_list, layer);
//	state_exit_layer_set();
//	return states_ok;
}
