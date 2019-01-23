/*
 * states.h
 *
 *  Created on: Sep 10, 2017
 *      Author: alxhoff
 */

#ifndef LAYERS_H_
#define LAYERS_H_

#include "keymap.h"
#include "error.h"

states_err_t state_enter_layer_set();
states_err_t state_exit_layer_set();
states_err_t state_layer_set( keymap_list_t* layer_list );

#endif /* LAYERS_H_ */