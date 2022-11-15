/**
 * @author  Alexander Hoffman
 * @email   alxhoff@gmail.com
 * @website http://alexhoffman.info
 * @license GNU GPL v3
 * @brief   STM32 HAL library for LCD screens using the SSD1306 controller
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

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void ssd1306_fill(void);
signed char ssd1306_update_screen(void);
void ssd1306_refresh(char **buf, unsigned char cursor_on, int cursor_x,
		int cursor_y);
void ssd1306_draw_cursor(unsigned char state, int x, int y);
signed char ssd1306_init(void);
void ssd1306_mv_cursor_left(void);
void ssd1306_mv_cursor_right(void);
unsigned char ssd1306_get_cols(void);
unsigned char ssd1306_get_rows(void);
