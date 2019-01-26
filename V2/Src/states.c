/*
 * states.c
 *
 *  Created on: 21 Jan 2019
 *      Author: alxhoff
 */

#include "states.h"


//#define ADD_STATE( MACHINE, STATE )	\
//	MACHINE = realloc(MACHINE, sizeof(state_t) * MACHINE->count);	\
//	MACHINE[MACHINE->count] = STATE;

unsigned char run_FSM(state_machine_t *FSM)
{
	if(FSM->next_state->ID != FSM->current_state->ID)
	{
		if(FSM->current_state->exit)  			/* Exit current state*/
			(FSM->current_state->exit)(FSM->current_state);
		if(FSM->next_state->enter)				/* Enter next state*/
			(FSM->next_state->enter)(FSM->next_state);
		FSM->current_state = FSM->next_state;	/* Change states*/
	}

	if(FSM->current_state->run)					/* Run current state*/
		(FSM->current_state->run)(FSM->current_state);

	return 0;
}

void *get_state_data(state_t *state)
{
	return state->data;
}

void set_state_data(state_t *state, void *data)
{
	state->data = data;
}

unsigned char init_FSM(state_machine_t *FSM)
{
	for(int i = 0; i < FSM->count; i++)
		if((FSM->states[i].init)())
			return 1;

	return 0;
}
