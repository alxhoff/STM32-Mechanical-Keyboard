/*
 * datatypes.h
 *
 *  Created on: Sep 19, 2017
 *      Author: alxhoff
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdio.h>

#include "error.h"
//#include "mouse.h"

//MACROS
//LCD


typedef struct keyboard_device keyboard_device_t;
typedef struct keyboard_HID_data keyboard_HID_data_t;
typedef struct keymap_list keymap_list_t;
typedef struct macro_table macro_table_t;
typedef struct shift_array shift_array_t;
typedef struct mouse_device mouse_device_t;
typedef struct mouse_HID_data mouse_HID_data_t;
typedef struct SSD1306_device SSD1306_device_t;
typedef struct screen screen_t;
typedef struct LED_array LED_array_t;

typedef struct key_devices key_devices_t;

struct key_devices{
	//keyboard

	keyboard_device_t* keyboard;
	keyboard_HID_data_t* keyboard_HID;

	//layers
	keymap_list_t* layer_list;

	//macros
	macro_table_t* macro_table;

	//usb dev

//	//mouse
	mouse_device_t* mouse;
	mouse_HID_data_t* mouse_HID;
//
//	//shift array
	shift_array_t* shift_array;

	//LCD
	SSD1306_device_t* LCD;

	//LCD contents
	screen_t* screen;

	LED_array_t* LEDs;

	uint8_t initialized;
};

int8_t keyboard_devices_init(key_devices_t** key_devs);

#endif /* DATATYPES_H_ */
