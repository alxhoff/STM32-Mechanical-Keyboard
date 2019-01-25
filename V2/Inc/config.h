/*
 * keyboard_config.h
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */


#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f4xx_hal.h"

//KEYBOARD SIZE
#define KEYBOARD_ROWS 		5
#define KEYBOARD_COLS 		15

/**
 * @brief Length of input buffer used when scanning the keyboard
 * */
#define SCAN_KEY_BUFFER_LENGTH		20
#define SCAN_KEY_PIPE_LENGTH		20

#define SHIFT_DEVICES		2

#define COLPORT(Col)		ROW_PORT_##Col
#define COLPIN(Col)			ROW_PIN_##Col

#define ROWPORT(Row)		ROW_PORT_##Row
#define ROWPIN(Row)			ROW_PIN_##Row

#define ROW_PORT_0 			GPIOD
#define ROW_PIN_0  			GPIO_PIN_7
#define ROW_PORT_1			GPIOE
#define ROW_PIN_1			GPIO_PIN_0
#define ROW_PORT_2			GPIOB
#define ROW_PIN_2			GPIO_PIN_11
#define ROW_PORT_3			GPIOB
#define ROW_PIN_3			GPIO_PIN_10
#define ROW_PORT_4			GPIOG
#define ROW_PIN_4			GPIO_PIN_9

#define CAPS_STATUS_PORT	GPIOC
#define CAPS_STATUS_PIN		GPIO_PIN_9
#define CLI_STATUS_PORT		GPIOC
#define CLI_STATUS_PIN		GPIO_PIN_10
#define FUNC_STATUS_PORT	GPIOC
#define FUNC_STATUS_PIN		GPIO_PIN_11

#define SHIFT_SER_IN_PORT	GPIOG
#define SHIFT_SER_IN_PIN	GPIO_PIN_1
#define SHIFT_SER_CLK_PORT	GPIOE
#define SHIFT_SER_CLK_PIN	GPIO_PIN_6
#define SHIFT_LATCH_PORT	GPIOE
#define SHIFT_LATCH_PIN		GPIO_PIN_3
#define SHIFT_OUT_ENA_PORT	GPIOE
#define SHIFT_OUT_ENA_PIN	GPIO_PIN_4
#define SHIFT_SER_CLR_PORT	GPIOE
#define SHIFT_SER_CLR_PIN	GPIO_PIN_5

//STATE CHANGE KEYS

#define HID_KEYBOARD_SC_LAYER_FUNCTION					  0xFF
#define HID_KEYBOARD_SC_MACRO_RUN_FUNCTION				  0xFE
#define	HID_KEYBOARD_SC_MACRO_SET_FUNCTION				  0xFC
#define HID_KEYBOARD_SC_CLI_FUNCTION					  0xFB

#endif /* CONFIG_H_ */
