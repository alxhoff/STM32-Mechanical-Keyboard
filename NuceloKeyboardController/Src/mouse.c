/*
 * mouse.c
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#include "mouse.h"
#include "keyboard.h"
#include "fonts.h"
#include "extern.h"
#include "ssd1306.h"
#include "stm32f4xx_hal.h"

key_err_TypeDef mouse_init( mouse_HID_data_t* data ){

	data->mouse_buf.x=0;
	data->mouse_buf.y=0;
	data->mouse_state=inactive;

//	calibrate_mouse(data);

	return mouse_ok;
}

key_err_TypeDef ADC_retrieve_values( mouse_device_t* mouse, mouse_HID_data_t* data )
{
	if(HAL_ADC_Start(mouse->adc_x) != HAL_OK)
		return adc_err;

	if(HAL_ADC_Start(mouse->adc_y) != HAL_OK)
		return adc_err;

	if(HAL_ADC_PollForConversion(mouse->adc_x, 50) != HAL_OK)
		return adc_err;

	if(HAL_ADC_PollForConversion(mouse->adc_y, 50) != HAL_OK)
		return adc_err;

	data->mouse_buf.x = ((signed long)HAL_ADC_GetValue(mouse->adc_x) >> 4);
	data->mouse_buf.y = ((signed long)HAL_ADC_GetValue(mouse->adc_y) >> 4);

	if((data->mouse_buf.x < 187 && data->mouse_buf.x > 147) && (data->mouse_buf.y < 187 && data->mouse_buf.y > 147))
			return no_mouse_mov;

	data->mouse_state = active;

	return mouse_ok;
}

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

key_err_TypeDef calibrate_mouse( mouse_HID_data_t* data )
{
	//TODO

	return key_ok;
}

key_err_TypeDef clear_mouse_report ( mouse_HID_data_t* data )
{
	if(data->mouse_state == clearing || data->mouse_state == active){
		data->mouse_HID.buttons=0;
		data->mouse_HID.x=0;
		data->mouse_HID.y=0;
		data->mouse_HID.wheel=0;
	}
	return mouse_ok;
}

key_err_TypeDef process_mouse_buf ( mouse_HID_data_t* data )
{
	//TODO MOUSE DYNAMICS - exponential

	//scale left and downwards ranges
	if(data->mouse_buf.x <= 155)
		data->mouse_buf.x *= 1.37;

	if(data->mouse_buf.y <= 155)
		data->mouse_buf.y *= 1.26;

	//shift
	data->mouse_buf.x -= 167;
	data->mouse_buf.y -= 167;

	//clear movements under 20 from center

	if(data->mouse_buf.x < 20 && data->mouse_buf.x > -20)
		data->mouse_buf.x = 0;

	if(data->mouse_buf.y < 20 && data->mouse_buf.y > -20)
		data->mouse_buf.y = 0;

	return mouse_ok;
}

key_err_TypeDef prepare_mouse_report ( mouse_HID_data_t* data )
{
	//basic mouse control
	data->mouse_HID.x = (uint8_t) data->mouse_buf.x;
	data->mouse_HID.y = -1 * (uint8_t) data->mouse_buf.y;

	return mouse_ok;
}

key_err_TypeDef send_mouse_report ( mouse_HID_data_t* data )
{
	USBD_HID_SendReport(&hUsbDeviceFS, &data->mouse_HID, sizeof(mouseHID_t));
	data->mouse_state = clearing;
	return mouse_ok;
}

key_err_TypeDef process_mouse_flags ( mouse_HID_data_t* data )
{
	if(data->mouse_state == active){
		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			prepare_mouse_report(data);
			send_mouse_report(data);
			data->mouse_state = clearing;

			xSemaphoreGive(USB_send_lock);
		}

	}else if( data->mouse_state == clearing){
		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			send_mouse_report(data);
			data->mouse_state = inactive;

			xSemaphoreGive(USB_send_lock);
		}
	}
	return mouse_ok;
}
