/*
 * extern.h
 *
 *  Created on: Aug 20, 2017
 *      Author: alxhoff
 */

#ifndef EXTERN_H_
#define EXTERN_H_

#include <config.h>
#include "cmsis_os.h"
#include "usb_device.h"
#include "keyboard.h"

extern GPIO_TypeDef* col_ports[KEYBOARD_COLS];
extern GPIO_TypeDef* row_ports[KEYBOARD_ROWS];
extern USBD_HandleTypeDef hUsbDeviceFS;
extern I2C_HandleTypeDef hi2c2;
extern SemaphoreHandle_t USB_send_lock;
extern key_devices_t* keyboard_devs;

#endif /* EXTERN_H_ */
