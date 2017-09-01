/*
 * keymap.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include "keymap.h"

const uint8_t keymap[][KEYBOARD_COLS][KEYBOARD_ROWS] = {
    /* 0: qwerty */
    KEYMAP(7, 8, 9, \
           4, 5, 6, \
           1, 2, 3, \
		   0),
};
