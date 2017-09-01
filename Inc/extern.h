/*
 * extern.h
 *
 *  Created on: Aug 20, 2017
 *      Author: alxhoff
 */

#ifndef EXTERN_H_
#define EXTERN_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "usb_device.h"

extern GPIO_TypeDef* col_ports[KEYBOARD_COLS];
extern GPIO_TypeDef* row_ports[KEYBOARD_ROWS];
extern USBD_HandleTypeDef hUsbDeviceFS;
extern I2C_HandleTypeDef hi2c2;

#endif /* EXTERN_H_ */
