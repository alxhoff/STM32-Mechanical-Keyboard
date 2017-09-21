/*
 * lcd.c
 *
 *  Created on: Sep 21, 2017
 *      Author: alxhoff
 */

#include "ssd1306.h"

void ADC_display_values(signed long* x, signed long* y)
{
	static char adc_stringx[10];
	static char adc_stringy[10];

	sprintf(adc_stringx, "X: %ld", *x);
	sprintf(adc_stringy, "Y: %ld", *y);

	ssd1306_Fill(White);
	ssd1306_SetCursor(23,10);
	ssd1306_WriteString(&adc_stringx,Font_11x18,Black);
	ssd1306_SetCursor(23,30);
	ssd1306_WriteString(&adc_stringy,Font_11x18,Black);
	ssd1306_UpdateScreen();
}


