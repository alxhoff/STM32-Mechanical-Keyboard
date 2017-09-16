/*
 * screen.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "screen.h"

void screen_draw_cursor(screen_t* screen)
{

}

void screen_render_two_line(screen_t* screen)
{
	//copy lines into output buffers
	strncpy(screen->line1_buf, &screen->line1_long[0], (strlen(screen->line1_long)>11)?strlen(screen->line1_long):11);
	strncpy(screen->line2_buf, &screen->line2_long[0], (strlen(screen->line2_long)>11)?strlen(screen->line2_long):11);
	strncpy(screen->line3_buf, &screen->line3_long[screen->cursor_x], ((strlen(screen->line3_long)-screen->cursor_x)<11)?strlen(screen->line3_long):11);


	ssd1306_Fill(White);
	ssd1306_SetCursor(4,3);
	ssd1306_WriteString(screen->line1_buf,*screen->font,Black);
	ssd1306_SetCursor(4,23);
	ssd1306_WriteString(screen->line2_buf,*screen->font,Black);
	ssd1306_SetCursor(4,43);
	ssd1306_WriteString(screen->line3_buf,*screen->font,Black);
	ssd1306_UpdateScreen();
}
