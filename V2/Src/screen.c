/*

 * screen.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff


#include <stdlib.h>
#include <string.h>

#include "screen.h"

void screen_draw_cursor(screen_t* screen)
{

}

void screen_render_two_line(screen_t* screen)
{
//	//copy lines into output buffers
//	strncpy(screen->line1_buf, &screen->line1_long[0],
//			(strlen(screen->line1_long) > 11) ?
//					strlen(screen->line1_long) : 11);
//	strncpy(screen->line2_buf, &screen->line2_long[0],
//			(strlen(screen->line2_long) > 11) ?
//					strlen(screen->line2_long) : 11);
//	strncpy(screen->line3_buf, &screen->line3_long[screen->cursor_x],
//			((strlen(screen->line3_long) - screen->cursor_x) < 11) ?
//					strlen(screen->line3_long) : 11);
//
//	ssd1306_Fill(White);
//	ssd1306_SetCursor(4, 3);
//	ssd1306_WriteString(screen->line1_buf, *screen->font, Black);
//	ssd1306_SetCursor(4, 23);
//	ssd1306_WriteString(screen->line2_buf, *screen->font, Black);
//	ssd1306_SetCursor(4, 43);
//	ssd1306_WriteString(screen->line3_buf, *screen->font, Black);
//	ssd1306_UpdateScreen();
}

void ADC_display_values(signed long* x, signed long* y)
{
//	static char adc_stringx[10];
//	static char adc_stringy[10];
//
//	sprintf(adc_stringx, "X: %ld", *x);
//	sprintf(adc_stringy, "Y: %ld", *y);
//
//	ssd1306_Fill(White);
//	ssd1306_SetCursor(23, 10);
//	ssd1306_WriteString(&adc_stringx, Font_11x18, Black);
//	ssd1306_SetCursor(23, 30);
//	ssd1306_WriteString(&adc_stringy, Font_11x18, Black);
//	ssd1306_UpdateScreen();
}

HAL_StatusTypeDef ssd1306_draw_cursor(screen_t* self)
{
	uint32_t i, b, j;

	char ch = '_';

	//TODO cursor check
	if (self->LCD_dev->width <= (self->LCD_dev->x + self->font->FontWidth)
			|| self->LCD_dev->height <= (self->LCD_dev->y + self->font->FontHeight))
	{
		return HAL_ERROR;
	}

	//cursor position
//	self->cursor_x = 3;
	uint8_t cursor_x = self->x_offset + self->font->FontWidth * self->cursor_x;
	uint8_t cursor_y =
			self->LCD_dev->height - (self->y_offset + self->font->FontHeight);


	if(self->cursor_on){
		for (i = 0; i < self->font->FontHeight; i++)
		{
			b = self->font->data[(ch - 32) * self->font->FontHeight + i];
			for (j = 0; j < self->font->FontWidth; j++)
			{
				if ((b << j) & 0x8000)
				{
					if (ssd1306_draw_pixel(self->LCD_dev,
							cursor_x + j,
							cursor_y + i,
							!self->LCD_dev->background) != HAL_OK)
						return HAL_ERROR;
				}
				else
				{
					if (ssd1306_draw_pixel(self->LCD_dev,
							cursor_x + j,
							cursor_y + i,
							self->LCD_dev->background) != HAL_OK)
						return HAL_ERROR;
				}
			}
		}
	}

	return HAL_OK;
}

unsigned char screen_update(screen_t* screen)
{
	screen->LCD_dev->fill(screen->LCD_dev, screen->LCD_dev->background);

	uint8_t x_position = screen->x_offset;
	uint8_t y_position;

	for (uint8_t i = 0; i < screen->rows; i++)
	{
		y_position = screen->LCD_dev->height
						- (screen->y_offset + (screen->font->FontHeight * (i + 1)));
		if(i == 0){
			screen->LCD_dev->string(screen->LCD_dev,
					(char*)screen->buffers[i] + screen->x_buff_shift);
			screen->LCD_dev->cursor(screen->LCD_dev, x_position, y_position);

			goto skip_end;
		}


		screen->LCD_dev->string(screen->LCD_dev, screen->buffers[i]);

		skip_end: ;
	}
	ssd1306_draw_cursor(screen);

	screen->LCD_dev->update(screen->LCD_dev);

	return 0;
}

//TODO can this be done without global?
void screen_cursor_callback(TimerHandle_t xTimer)
{
//	keyboard_devs->screen->cursor_on = !keyboard_devs->screen->cursor_on;
//	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

screen_t* screen_init(screen_init_t* init_values)
{
	screen_t* init_dev = (screen_t*) calloc(1, sizeof(screen_t));

	if (init_dev == NULL)
		return NULL;

	init_dev->LCD_dev = init_values->LCD_dev;

	if (init_values->font != NULL)
		init_dev->font = init_values->font;

	init_dev->rows = init_values->rows;
	init_dev->cols = init_values->cols;

	init_dev->row_height = init_values->row_height;

	init_dev->x_offset = init_values->x_offset;
	init_dev->y_offset = init_values->y_offset;

//	init_dev->cursor_x = 0;
//	init_dev->cursor_y = 0;
//	init_dev->cursor_on = 0;
//	init_dev->x_buff_shift = 3;
	init_dev->cursor_period = init_values->cursor_period;

	//buffers
	if (init_values->cols <= 0 || init_values->rows <= 0)
		return NULL;

	init_dev->buffers = (char**) calloc(init_values->cols, sizeof(char*));
	if (init_dev->buffers == NULL)
		return NULL;

	//welcome message
	init_dev->buffers[0] = realloc(init_dev->buffers[0],
			strlen(init_values->message) + 1);

	if (init_dev->buffers[0] == NULL)
		return NULL;

	strcpy(init_dev->buffers[0], init_values->message);

	init_dev->update = &screen_update;
	init_dev->cursor_callback = &screen_cursor_callback;
	init_dev->draw_cursor = &ssd1306_draw_cursor;

	//timer init
	init_dev->cursor_timer = xTimerCreate("Cursor Timer",
			init_dev->cursor_period, 1, (void*) 0, init_dev->cursor_callback);

	xTimerStart(init_dev->cursor_timer, 0);

	return init_dev;
}
*/
