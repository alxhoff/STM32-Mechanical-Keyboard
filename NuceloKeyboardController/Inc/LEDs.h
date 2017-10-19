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

	//EFFECT PARAMS
	uint8_t rainbow_effect_length;
	uint8_t rainbow_delay;

	void (*update)(key_devices_t*);
} LED_array_t;

#endif /* LEDS_H_ */
