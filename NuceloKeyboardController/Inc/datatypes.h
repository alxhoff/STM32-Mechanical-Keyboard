/*
 * datatypes.h
 *
 *  Created on: Sep 19, 2017
 *      Author: alxhoff
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include "keyboard_config.h"
//#include "SN54HC595.h"
//#include "mouse.h"

typedef struct key_devices key_devices_t;


typedef struct mouse_device mouse_device_t;
typedef struct mouse_HID_data mouse_HID_data_t;
typedef struct shift_array shift_array_t;



key_err_TypeDef keyboard_devices_init(key_devices_t* key_devs);

#endif /* DATATYPES_H_ */
