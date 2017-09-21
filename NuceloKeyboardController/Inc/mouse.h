/*
 * mouse.h
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include <stdio.h>

#include "states.h"

#include "stm32f4xx_hal.h"
//#include "keyboard.h"

//CALIBRATION
#define LARGE_RANGE 	100
#define SMALL_RANGE 	84

//X
#define X_MIN			53
#define X_MAX			255
#define X_MID			164

//Y
#define Y_MIN			62
#define Y_MAX			255
#define Y_MID			178


typedef struct{
	const uint8_t id;
	uint8_t buttons;
	int8_t x;
	int8_t y;
	int8_t wheel;
  } mouseHID_t;

typedef struct mouse_buf{
	signed long x;
	signed long y;
} mouse_buf_t;

typedef struct mouse_HID_data{
	mouseHID_t mouse_HID;
	report_states mouse_state;

	mouse_buf_t mouse_buf;
} mouse_HID_data_t;

//typedef struct ADC_HandleTypeDef;

typedef struct mouse_device{
	const ADC_HandleTypeDef* adc_x;
	const ADC_HandleTypeDef* adc_y;

	signed long x_cal;
	signed long y_cal;
} mouse_device_t;

#endif /* MOUSE_H_ */
