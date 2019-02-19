/*
 * states.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef STATES_H_
#define STATES_H_

typedef enum{
	state_send,
	state_CLI,
	state_macro_run,
	state_macro_set
} state_e;

unsigned char states_init(void);
unsigned char states_run(void);
void *get_state_data(void);
void set_state_data(void *data);
void states_set_input(unsigned char input);
unsigned char states_get_state(void);
void states_set_state(unsigned char state);
unsigned char states_get_input(void);
void states_clear_input(void);

#endif /* STATES_H_ */
