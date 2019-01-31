/*
 * screen_API.h
 *
 *  Created on: 28 Jan 2019
 *      Author: alxhoff
 */

#ifndef SCREEN_API_H_
#define SCREEN_API_H_

#include "ssd1306.h"

//fill in these defines to provide the screen -> screen driver interface

#define SCREEN_GET_ROWS		ssd1306_get_rows();
#define SCREEN_GET_COLS		ssd1306_get_cols();
#define SCREEN_DRAW_TEXT	ssd1306_draw_text_buffer
#define SCREEN_DRAW_CURS	ssd1306_draw_cursor
#define SCREEN_MV_CUR_LEFT	ssd1306_mv_cursor_left
#define SCREEN_MV_CUR_RIGHT	ssd1306_mv_cursor_right
#define SCREEN_CLEAR		ssd1306_fill

#define SCREEN_REFRESH		ssd1306_update_screen
#endif /* SCREEN_API_H_ */
