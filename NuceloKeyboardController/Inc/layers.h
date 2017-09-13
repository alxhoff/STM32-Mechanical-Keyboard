/*
 * states.h
 *
 *  Created on: Sep 10, 2017
 *      Author: alxhoff
 */

#ifndef LAYERS_H_
#define LAYERS_H_

#include "states.h"
#include "keymap.h"

states_err_t state_exit_layer_set();
states_err_t state_layer_set( keymap_list_t* layer_list );

#endif /* LAYERS_H_ */
