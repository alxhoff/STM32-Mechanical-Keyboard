/*
 * macro.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef MACRO_H_
#define MACRO_H_

typedef struct macro_entry macro_entry_t;

struct macro_entry{
	uint8_t key_code;

	char* keypress_string;

	macro_entry_t* next;
	macro_entry_t* previous;
};

typedef struct{
	macro_entry_t* head;

	uint8_t count;
} macro_table;

#endif /* MACRO_H_ */
