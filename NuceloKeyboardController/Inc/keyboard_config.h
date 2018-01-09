/*
 * keyboard_config.h
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */


#ifndef KEYBOARD_CONFIG_H_
#define KEYBOARD_CONFIG_H_

//KEYBOARD SIZE
#define KEYBOARD_ROWS 5
#define KEYBOARD_COLS 15

//MICRO PORTS AND PINS
	//COLS

#define COLPORT(Col)		COL_PORT_##Col
#define COLPIN(Col)			COL_PIN_##Col

#define ROWPORT(Row)		ROW_PORT_##Row
#define ROWPIN(Row)			ROW_PIN_##Row

#define COL_PORT_0 			GPIOF
#define COL_PIN_0  			GPIO_PIN_8
#define COL_PORT_1			GPIOF
#define COL_PIN_1			GPIO_PIN_7
#define COL_PORT_2			GPIOF
#define COL_PIN_2			GPIO_PIN_9

#endif /* KEYBOARD_CONFIG_H_ */
