/*
 * macro.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef MACRO_H_
#define MACRO_H_

unsigned char macro_add_new_entry( char *string, unsigned char sc );

unsigned char macro_rem_entry(unsigned char sc);

unsigned char macro_run_sc(unsigned char sc);

//STATES
unsigned char macro_init(void);

void macro_enter(void);

void macro_run(void);

void macro_exit(void);

unsigned char macro_set_init(void);

void macro_set_enter(void);

void macro_set_run(void);

void macro_set_exit(void);

#endif /* MACRO_H_ */
