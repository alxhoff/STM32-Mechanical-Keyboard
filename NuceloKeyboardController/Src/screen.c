/*
 * screen.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "fonts.h"
#include "ssd1306.h"

void screen_display_2_line(char* input_str1, char* input_str2)
{
	ssd1306_Fill(White);
	ssd1306_SetCursor(23,10);
	ssd1306_WriteString(input_str1,Font_11x18,Black);
	ssd1306_SetCursor(23,30);
	ssd1306_WriteString(input_str2,Font_11x18,Black);
	ssd1306_UpdateScreen();
}
