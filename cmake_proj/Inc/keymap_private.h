/*
 * keymap_private.h
 *
 *  Created on: 26 Jan 2019
 *      Author: alxhoff
 */

#ifndef KEYMAP_PRIVATE_H_
#define KEYMAP_PRIVATE_H_

#include "config.h"

typedef struct keymap_layer keymap_layer_t;

struct keymap_layer{
//	key_grid_TypeDef* grid;
	unsigned char 		grid[KEYBOARD_ROWS][KEYBOARD_COLS];

	unsigned char 		mod_key_code;

	char* 				name;
	volatile unsigned char 		ID;

	keymap_layer_t* 	next;
};

#endif /* KEYMAP_PRIVATE_H_ */
