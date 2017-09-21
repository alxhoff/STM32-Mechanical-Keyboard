/*
 * datatypes.c
 *
 *  Created on: Sep 19, 2017
 *      Author: alxhoff
 */

#include <stdio.h>
#include "datatypes.h"

int8_t keyboard_devices_init(key_devices_t* key_devs)
{
	key_devs = (key_devices_t*)malloc(sizeof(key_devices_t));
	if(key_devs == NULL)
		return -1;

	key_devs->keyboard = NULL;
	key_devs->keyboard_HID = NULL;
	key_devs->layer_list = NULL;
	key_devs->mouse = NULL;
	key_devs->mouse_HID = NULL;
	key_devs->shift_array = NULL;

	return 0;
}
