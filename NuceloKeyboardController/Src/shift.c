/*
 * shift.c
 *
 *  Created on: Sep 19, 2017
 *      Author: alxhoff
 */

#include <stdlib.h>

#include "shift.h"

shift_err_t shift_init(key_devices_t* keyboard_devices, shift_array_t* shift_dev)
{
	keyboard_devices->shift_array = (shift_array_t*)calloc(1, sizeof(shift_array_t));
	if(keyboard_devices->shift_array == NULL)
		return shift_init_err;

	memcpy(keyboard_devices->shift_array, shift_dev, sizeof(shift_array_t));

	SN54HC595_init_obj(keyboard_devices->shift_array);

	return shift_ok;
}
