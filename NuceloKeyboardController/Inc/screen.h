/*
 * screen.h
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "fonts.h"
#include "ssd1306.h"

typedef struct screen screen_t;

struct screen{
	char line1_buf[12];
	char line2_buf[12];
	char line3_buf[12];

	char* line1_long;
	char* line2_long;
	char* line3_long;

	uint8_t rows;
	uint8_t cols;

	uint8_t cursor_x;
	uint8_t cursor_y;

	FontDef* font;
};

void screen_render_two_line();

#endif /* SCREEN_H_ */
