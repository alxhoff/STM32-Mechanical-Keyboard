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

struct scan_code
{
	uint8_t code;
	uint8_t mod;
};

struct scan_code_char
{
	char* unmodified;
	char* modified;
};

extern struct scan_code_char lookup_char[207];
extern struct scan_code lookup_sc[127];

#endif /* LOOKUP_H_ */
