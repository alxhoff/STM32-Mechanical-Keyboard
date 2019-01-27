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
} state_e;

unsigned char states_init(void);
unsigned char states_run(void);
void *get_state_data(void);
void set_state_data(void *data);

#endif /* STATES_H_ */
