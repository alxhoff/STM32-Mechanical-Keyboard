/*
 * mouse.c
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#include "mouse.h"
#include "error.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

mouse_device_t *mouse_dev;

typedef struct buf{
	signed long x;
	signed long y;
} buf_t;

typedef struct mouse_device{
	const ADC_HandleTypeDef* adc_x;
	const ADC_HandleTypeDef* adc_y;

	signed long x_cal;
	signed long y_cal;

	buf_t buf;

	report_states state;
} mouse_device_t;

unsigned char mouse_init(void)
{
	mouse_dev = calloc(1, sizeof(mouse_device_t));
	if(mouse_dev) return -ENOMEM;

	mouse_dev->buf.x=0;
	mouse_dev->buf.y=0;
	mouse_dev->state=inactive;

	return 0;
}

mouse_coord_t mouse_get_coord(void)
{
	mouse_coord_t ret = {0};

	if(HAL_ADC_Start(mouse_dev->adc_x) != HAL_OK)
		return NULL;

	if(HAL_ADC_Start(mouse_dev->adc_y) != HAL_OK)
		return NULL;

	if(HAL_ADC_PollForConversion(mouse_dev->adc_x, 50) != HAL_OK)
		return NULL;

	if(HAL_ADC_PollForConversion(mouse_dev->adc_y, 50) != HAL_OK)
		return NULL;

	ret.x = ((signed long)HAL_ADC_GetValue(mouse_dev->adc_x) >> 4);
	ret.y = ((signed long)HAL_ADC_GetValue(mouse_dev->adc_y) >> 4);

	//TODO what is this?
	mouse_dev->state = active;

	return 0;
}

unsigned char mouse_get_buttons(void)
{
	//TODO
	return 0;
}

signed char mouse_get_wheel(void)
{
	return 0;
}

unsigned char calibrate_mouse(void)
{
	//TODO

	return 0;
}



void process_buf ( mouse_HID_data_t* data )
{
	//TODO MOUSE DYNAMICS - exponential

	//scale left and downwards ranges
	if(mouse_dev->buf.x <= 155)
		mouse_dev->buf.x *= 1.37;

	if(mouse_dev->buf.y <= 155)
		mouse_dev->buf.y *= 1.26;

	//shift
	mouse_dev->buf.x -= 167;
	mouse_dev->buf.y -= 167;

	//clear movements under 20 from center

	if(mouse_dev->buf.x < 20 && mouse_dev->buf.x > -20)
		mouse_dev->buf.x = 0;

	if(mouse_dev->buf.y < 20 && mouse_dev->buf.y > -20)
		mouse_dev->buf.y = 0;
}

mouse_HID_t mouse_prepare_report (void)
{
	mouse_HID_t ret = {0};

	ret.buttons = mouse_get_buttons();
	ret.coord = mouse_get_coord();
	ret.wheel = mouse_get_buttons();

	return 0;
}

unsigned char mouse_send_report ( mouse_HID_t* data )
{
	USBD_HID_SendReport(&hUsbDeviceFS, &data, sizeof(mouse_HID_t));
	mouse_dev->state = clearing;
	return 0;
}

unsigned char mouse_run (void)
{
	static mouse_HID_t data = NULL;
	if(mouse_dev->state == active){
		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			data = mouse_prepare_report();
			mouse_send_report(&data);
			mouse_dev->state = clearing;

			xSemaphoreGive(USB_send_lock);
		}

	}else if( mouse_dev->state == clearing){
		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			mouse_send_report(data);
			mouse_dev->state = inactive;

			xSemaphoreGive(USB_send_lock);
		}
	}
	return 0;
}
