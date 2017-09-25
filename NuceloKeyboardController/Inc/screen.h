/*
 * screen.h
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "FreeRTOS.h"
#include "timers.h"

#include "error.h"
#include "fonts.h"
#include "ssd1306.h"

#define GET_SCREEN	keyboard_devs->screen
#define GET_LCD		keyboard_devs->screen->LCD_dev

typedef struct screen_init
{
	uint8_t rows;
	uint8_t cols;

	uint8_t row_height;
	uint8_t x_offset;
	uint8_t y_offset;

	int cursor_period;

	char* message;

	SSD1306_device_t* LCD_dev;

	FontDef* font;
} screen_init_t;

typedef struct screen screen_t;
struct screen
{
	char* line1_buf;
	char* line2_buf;
	char* line3_buf;

	char* line1_long;
	char* line2_long;
	char* line3_long;

	char** buffers;

	uint8_t rows;
	uint8_t cols;

	uint8_t row_height;

	uint16_t x_offset;
	uint16_t x_buff_shift;
	uint16_t y_offset;
	uint16_t y_buff_shift;

	uint8_t cursor_x;
	uint8_t cursor_y;
	uint8_t cursor_on;
	TimerHandle_t cursor_timer;
	int cursor_period;

	void (*cursor_callback)(TimerHandle_t);

	FontDef* font;

	SSD1306_device_t* LCD_dev;

	screen_err_t (*update)(screen_t*);
	HAL_StatusTypeDef (*draw_cursor)(screen_t*);
};

void screen_render_two_line();
void ADC_display_values(signed long* x, signed long* y);
screen_t* screen_init(screen_init_t* init_values);

#endif /* SCREEN_H_ */
