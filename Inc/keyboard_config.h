/*
 * keyboard_config.h
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */


#ifndef KEYBOARD_CONFIG_H_
#define KEYBOARD_CONFIG_H_

#include "stm32f4xx_hal.h"

//KEYBOARD SIZE
#define KEYBOARD_ROWS 4
#define KEYBOARD_COLS 3

//MICRO PORTS AND PINS
	//COLS

#define COLPORT(Col)		COL_PORT_##Col
#define COLPIN(Col)			COL_PIN_##Col

//#define rowPort(row)	COL_##_PORT
//#define rowPin(row)		COL_##_PIN

#define COL_PORT_0 			GPIOD
#define COL_PIN_0  			LD3
#define COL_PORT_1
#define COL_PIN_1
#define COL_PORT_2
#define COL_PIN_2

#define ROW_PORT_0
#define ROW_PIN_0
#define ROW_PORT_1
#define ROW_PIN_1
#define ROW_PORT_2
#define ROW_PIN_2
#define ROW_PORT_3
#define ROW_PIN_3


#endif /* KEYBOARD_CONFIG_H_ */
