/*
 * LEDs.h
 *
 *  Created on: Oct 19, 2017
 *      Author: alxhoff
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <config.h>

#define CHECK_EVENODD(N) ((N%2==0)?1:0)

#if CHECK_EVENODD(KEYBOARD_COLS)
#define LED_ROW_BUFF_LENGTH 	KEYBOARD_COLS
#else
#define LED_ROW_BUFF_LENGTH		(KEYBOARD_COLS + 1)
#endif

typedef struct LED_array{
	uint8_t buffers[KEYBOARD_ROWS][LED_ROW_BUFF_LENGTH*3];

	uint8_t inverted_rows;

	//EFFECT PARAMS
	uint8_t rainbow_effect_length;
	uint32_t rainbow_delay;

	uint32_t dots_fade_out;
	uint32_t dots_random;

	uint8_t solid_red;
	uint8_t solid_green;
	uint8_t solid_blue;

	uint8_t matrix_red;
	uint8_t matrix_green;
	uint8_t matrix_blue;
	uint8_t matrix_probability; //out of 100
	uint8_t matrix_fade_amount;
	uint32_t matrix_delay;

	uint8_t set_row_number;
	uint32_t set_row_colour;

	uint32_t count_row_number;
	uint32_t count_delay;
	uint32_t count_row_colour;
	uint8_t count_col_indexs[KEYBOARD_ROWS];
	uint8_t count_row_index;
	uint8_t count_row_direction;
	uint8_t count_col_direction;

	void (*update)(void);
} LED_array_t;

#endif /* LEDS_H_ */
