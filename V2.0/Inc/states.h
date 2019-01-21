/*
 * states.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef STATES_H_
#define STATES_H_

//KEYBOARD STATES

typedef enum{
	typing,
	layer_set,
	macro_set,
	macro_run,
	CLI
} keyboard_states;

typedef enum{
	inactive,
	active,
	clearing
} report_states;

extern keyboard_states current_keyboard_state;

#endif /* STATES_H_ */
