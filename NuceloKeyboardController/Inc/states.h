/*
 * states.h
 *
 *  Created on: Sep 10, 2017
 *      Author: alxhoff
 */

#ifndef STATES_H_
#define STATES_H_

//KEYBOARD STATES
typedef enum{
	states_ok		= 0
} states_err_typeDef;

typedef enum{
	typing,
	layer_set
} keyboard_states;

extern keyboard_states current_keyboard_state;

#endif /* STATES_H_ */
