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

unsigned char macro_add_new_entry( char *string, scan_code_t *sc );

void macro_add_entry(macro_entry_t *entry);

unsigned char macro_rem_entry(scan_code_t *sc);

macro_entry_t* macro_get_sc(scan_code_t *sc);

unsigned char macro_run_sc(scan_code_t *sc);

#endif /* MACRO_H_ */
