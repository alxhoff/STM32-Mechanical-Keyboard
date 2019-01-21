/*
 * states.c
 *
 *  Created on: 21 Jan 2019
 *      Author: alxhoff
 */

#include "states.h"

#include <stddef.h>

unsigned char runFSM(state_machine_t *FSM)
{
	if(FSM->next_state->ID != FSM->current_state->ID)
	{
		if(FSM->current_state->exit)  		/* Exit current state*/
			(FSM->current_state->exit)(FSM->current_state);
		if(FSM->next_state->enter)
			(FSM->next_state->enter)(FSM->next_state);
		FSM->current_state = FSM->next_state;
	}

	if(FSM->current_state->run)
		(FSM->current_state->run)(FSM->current_state);

	return 0;
}
