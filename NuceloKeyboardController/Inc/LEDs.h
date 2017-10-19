/*
 * LEDs.h
 *
 *  Created on: Oct 19, 2017
 *      Author: alxhoff
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "keyboard_config.h"


typedef struct LED_array{
	uint8_t buffers[KEYBOARD_ROWS][KEYBOARD_COLS*3];

} LED_array_t;

#endif /* LEDS_H_ */
