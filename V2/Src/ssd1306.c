/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief
 *
 @verbatim
 ----------------------------------------------------------------------
 Copyright (C) Alexander Hoffman, 2017

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ----------------------------------------------------------------------
 @endverbatim
 */

#include <stdlib.h>

#include "fonts.h"
#include "ssd1306.h"
#include "error.h"
#include "config.h"
#ifdef FREERTOS
#include "cmsis_os.h"
#endif

#define SSD1306_I2C_ADDR        0x78
#define SSD1306_BACKGROUND		0
#define SSD1306_FONT			&Font_11x18

#define SSD1306_WIDTH			128
#define SSD1306_CHAR_WIDTH		ssd1306_dev.font->FontWidth
#define SSD1306_WIDTH_CHARS		SSD1306_WIDTH / SSD1306_CHAR_WIDTH

#define SSD1306_HEIGHT			64
#define SSD1306_CHAR_HEIGHT		ssd1306_dev.font->FontHeight
#define SSD1306_HEIGHT_CHARS	SSD1306_HEIGHT / SSD1306_CHAR_HEIGHT

#define SSD1306_X_OFFSET		5
#define SSD1306_Y_OFFSET		5

typedef enum {
	Black = 0x00, /*!< Black color, no pixel */
	White = 0x01 /*!< Pixel is set. Color depends on LCD */
} SSD1306_colour_t;

typedef struct ssd1306_device ssd1306_device_t;

struct ssd1306_device {
	uint16_t x;
	uint16_t y;
	unsigned char cursor_pos;

	uint8_t initialized;

	SSD1306_colour_t background;

	FontDef* font;

	uint8_t width;
	uint8_t height;

	uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

	I2C_HandleTypeDef* port;

	void (*clear)(void);
	unsigned char (*update)(void);
	void (*fill)(void);
	void (*string)(char*);
};

ssd1306_device_t ssd1306_dev = { 0 };

unsigned char ssd1306_get_cols(void) {
	return SSD1306_WIDTH_CHARS;
}

unsigned char ssd1306_get_rows(void) {
	return SSD1306_HEIGHT_CHARS;
}

unsigned char ssd1306_write_command(uint8_t command) {
	if (HAL_I2C_Mem_Write(ssd1306_dev.port, SSD1306_I2C_ADDR, 0x00, 1, &command,
			1, 10) != 0)
		return -EWRITE;

	return 0;
}

void ssd1306_fill(void) {
	for (int i = 0; i < sizeof(ssd1306_dev.buffer); i++) {
		ssd1306_dev.buffer[i] = (ssd1306_dev.background == Black) ? 0xFF : 0x00;
	}
}

unsigned char ssd1306_update_screen(void) {
	uint8_t i;

	for (i = 0; i < 8; i++) {
		ssd1306_write_command(0xB0 + i);
		ssd1306_write_command(0x00);
		ssd1306_write_command(0x10);

		if (HAL_I2C_Mem_Write(ssd1306_dev.port, SSD1306_I2C_ADDR, 0x40, 1,
				&ssd1306_dev.buffer[ssd1306_dev.width * i], ssd1306_dev.width,
				100) != 0)
			return -EWRITE;
	}
	return 0;
}

void ssd1306_clear(void) {
	ssd1306_fill();
	ssd1306_update_screen();
}

void ssd1306_set_pixel(unsigned char x, unsigned char y) {
	ssd1306_dev.x = x;
	ssd1306_dev.y = y;
}

void ssd1306_set_draw_cursor(int x, int y) {

	if (x >= (SSD1306_WIDTH - SSD1306_CHAR_WIDTH)) /* X greater than screen width */
		ssd1306_dev.x = SSD1306_WIDTH - SSD1306_CHAR_WIDTH;
	else if (x < SSD1306_X_OFFSET)
		ssd1306_dev.x = SSD1306_X_OFFSET;
	else
		ssd1306_dev.x = x;

	if (y > (SSD1306_HEIGHT - SSD1306_CHAR_HEIGHT))
		ssd1306_dev.y = SSD1306_HEIGHT - SSD1306_CHAR_HEIGHT;
	else if (y < SSD1306_Y_OFFSET)
		ssd1306_dev.y = SSD1306_Y_OFFSET;
	else
		ssd1306_dev.y = y;
}

void ssd1306_mv_cursor_left(void) {
	if (ssd1306_dev.cursor_pos > 0)
		ssd1306_dev.cursor_pos--;
}

void ssd1306_mv_cursor_right(void) {
	if (ssd1306_dev.cursor_pos < SSD1306_WIDTH_CHARS)
		ssd1306_dev.cursor_pos++;
}

unsigned char ssd1306_draw_pixel(uint8_t x, uint8_t y, SSD1306_colour_t colour) {
	if (x >= ssd1306_dev.width || y >= ssd1306_dev.height) {
		return -EBOUNDS;
	}

	if (colour == Black) {
#ifndef	SCREEN_INVERTED
		ssd1306_dev.buffer[(SSD1306_WIDTH * SSD1306_HEIGHT / 8)
				- (x + (y / 8) * ssd1306_dev.width)] |= 1 << (7 - (y % 8));
	} else {
		ssd1306_dev.buffer[(SSD1306_WIDTH * SSD1306_HEIGHT / 8)
				- (x + (y / 8) * ssd1306_dev.width)] &= ~(1 << (7 - (y % 8)));
#else
		ssd1306_dev.buffer[x + (y / 8) * ssd1306_dev.width] |= 1 << (y % 8);
	} else {
		ssd1306_dev.buffer[x + (y / 8) * ssd1306_dev.width] &= ~(1 << (y % 8));
#endif
	}

	return 0;
}

unsigned char ssd1306_invert_pixel(uint8_t x, uint8_t y) {
	if (x >= ssd1306_dev.width || y >= ssd1306_dev.height) {
		return -EBOUNDS;
	}
#ifndef	SCREEN_INVERTED
	ssd1306_dev.buffer[(SSD1306_WIDTH * SSD1306_HEIGHT / 8)
			- (x + (y / 8) * ssd1306_dev.width)] ^= 1 << (7 - (y % 8));
	;
#else
	ssd1306_dev.buffer[x + (y / 8) * ssd1306_dev.width] ^= 1 << (y % 8);;
#endif
	return 0;
}

void ssd1306_write_char(char ch) {
	static unsigned short b;

	if (ssd1306_dev.width <= (ssd1306_dev.x + ssd1306_dev.font->FontWidth) /* would print outside of bounds */
	|| ssd1306_dev.height <= (ssd1306_dev.y + ssd1306_dev.font->FontHeight)) {
		return;
	}

	for (unsigned char i = 0; i < ssd1306_dev.font->FontHeight; i++) {
		b =
				ssd1306_dev.font->data[(ch - 32) * ssd1306_dev.font->FontHeight
						+ i];
		for (unsigned char j = 0; j < ssd1306_dev.font->FontWidth; j++)
			if ((b << j) & 0x8000)
				ssd1306_draw_pixel(ssd1306_dev.x + j, (ssd1306_dev.y + i),
						(SSD1306_colour_t) !ssd1306_dev.background);
			else
				ssd1306_draw_pixel(ssd1306_dev.x + j, (ssd1306_dev.y + i),
						(SSD1306_colour_t) ssd1306_dev.background);
	}

	ssd1306_dev.x += ssd1306_dev.font->FontWidth;
}

unsigned char ssd1306_invert_box(unsigned char x) {

	for (unsigned char i = x * SSD1306_CHAR_WIDTH + SSD1306_X_OFFSET - 1;
			i < x * SSD1306_CHAR_WIDTH + SSD1306_X_OFFSET + SSD1306_CHAR_WIDTH;
			i++)
		for (unsigned char j = SSD1306_Y_OFFSET - 1;
				j < SSD1306_Y_OFFSET + SSD1306_CHAR_HEIGHT - 1; j++)
			if (ssd1306_invert_pixel(i, j) != 0)
				return -EWRITE;
	return 0;
}

void ssd1306_write_string(char* str) {
	while (*str) {
		ssd1306_write_char(*str);
		str++;
	}
}

void ssd1306_draw_framebuffer(char **buf) {
	for (unsigned char i = 0; i < SSD1306_HEIGHT_CHARS; i++) {
		ssd1306_set_draw_cursor(SSD1306_X_OFFSET,
		SSD1306_Y_OFFSET + i * SSD1306_CHAR_HEIGHT);
		ssd1306_write_string(buf[i]);
	}
}

unsigned char ssd1306_init(void) {
	//functions
	ssd1306_dev.clear = &ssd1306_clear;
	ssd1306_dev.update = &ssd1306_update_screen;
	ssd1306_dev.fill = &ssd1306_fill;
	ssd1306_dev.string = &ssd1306_write_string;

	ssd1306_dev.port = &SSD1306_I2C_PORT;

	ssd1306_dev.width = SSD1306_WIDTH;
	ssd1306_dev.height = SSD1306_HEIGHT;

	ssd1306_dev.background = SSD1306_BACKGROUND;
	ssd1306_dev.font = SSD1306_FONT;

	/* Init LCD */
	ssd1306_write_command(0xAE); //display off
	ssd1306_write_command(0x20); //memory addressing mode
	ssd1306_write_command(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_write_command(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_write_command(0xC8); //Set COM Output Scan Direction
	ssd1306_write_command(0x00); //---set low column address
	ssd1306_write_command(0x10); //---set high column address
	ssd1306_write_command(0x40); //--set start line address
	ssd1306_write_command(0x81); //--set contrast control register
	ssd1306_write_command(0xFF);
	ssd1306_write_command(0xA1); //--set segment re-map 0 to 127
	ssd1306_write_command(0xA6); //--set normal display
	ssd1306_write_command(0xA8); //--set multiplex ratio(1 to 64)
	ssd1306_write_command(0x3F); //
	ssd1306_write_command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_write_command(0xD3); //-set display offset
	ssd1306_write_command(0x00); //-not offset
	ssd1306_write_command(0xD5); //--set display clock divide ratio/oscillator frequency
	ssd1306_write_command(0xF0); //--set divide ratio
	ssd1306_write_command(0xD9); //--set pre-charge period
	ssd1306_write_command(0x22); //
	ssd1306_write_command(0xDA); //--set com pins hardware configuration
	ssd1306_write_command(0x12);
	ssd1306_write_command(0xDB); //--set vcomh
	ssd1306_write_command(0x20); //0x20,0.77xVcc
	ssd1306_write_command(0x8D); //--set DC-DC enable
	ssd1306_write_command(0x14); //
	ssd1306_write_command(0xAF); //--turn on SSD1306 panel

	ssd1306_clear();
	ssd1306_clear();

	ssd1306_dev.x = 0;
	ssd1306_dev.y = 0;

	ssd1306_dev.initialized = 1;

	return 0;
}

void ssd1306_draw_cursor(unsigned char state) {
	if (state)
		ssd1306_invert_box(
		SSD1306_X_OFFSET * ssd1306_dev.cursor_pos * SSD1306_CHAR_WIDTH);
}

void ssd1306_draw_text_buffer(char **buf) {
	ssd1306_draw_framebuffer(buf);
}

