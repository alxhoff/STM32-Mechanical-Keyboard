/*
 * macro.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef MACRO_H_
#define MACRO_H_

#include "lookup.h"

typedef struct macro_entry macro_entry_t;

struct macro_entry{
	scan_code_t sc;

	char* string;

	macro_entry_t *next;
};


unsigned char macro_init(void);


#endif /* MACRO_H_ */
