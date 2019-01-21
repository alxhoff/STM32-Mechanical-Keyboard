/*
 * states.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef STATES_H_
#define STATES_H_

//KEYBOARD STATES

#define ADD_STATE( STATE_STRUCT )

typedef enum{
	typing,
	layer_set,
	macro_set,
	macro_run,
	CLI
} keyboard_states_t;

typedef struct state state_t;
struct state{
	unsigned char 	ID;

	void*			data;

	char* 			name;

	unsigned char	(*init)(void);

	unsigned char 	(*enter)(state_t*);
	unsigned char 	(*run)(state_t*);
	unsigned char 	(*exit)(state_t*);
};

typedef struct state_machine{
	state_t			*current_state;
	state_t			*next_state;

	state_t			*state_head;
} state_machine_t;

typedef enum{
	inactive,
	active,
	clearing
} report_states;

extern keyboard_states_t current_keyboard_state;

#endif /* STATES_H_ */
