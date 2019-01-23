/*
 * lookup.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef LOOKUP_H_
#define LOOKUP_H_

#include <stdio.h>

#define SC(key) HID_KEYBOARD_SC_##key
#define MOD(key) HID_KEYBOARD_MODIFIER_##key

typedef struct scan_code
{
	unsigned char code;
	unsigned char mod;
} scan_code_t;

struct scan_code_char
{
	char* unmodified;
	char* modified;
};


#endif /* LOOKUP_H_ */
