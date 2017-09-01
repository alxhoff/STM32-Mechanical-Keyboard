/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "stm32f4xx_hal.h"

//KEYBOARD SIZE
#define KEYBOARD_ROWS 3
#define KEYBOARD_COLUMNS 4

//MICRO PORTS AND PINS
	//COLS

#define COLPORT(Col)		COL_PORT_##Col
#define COLPIN(Col)			COL_PIN_##Col

#define ROWPORT(Row)		ROW_PORT_##Row
#define ROWPIN(Row)			ROW_PIN_##Row

#define COL_PORT_0 			GPIOE
#define COL_PIN_0  			COL0_Pin
#define COL_PORT_1			GPIOE
#define COL_PIN_1			COL1_Pin
#define COL_PORT_2			GPIOE
#define COL_PIN_2			COL2_Pin
#define COL_PORT_3			GPIOE
#define COL_PIN_3			COL3_Pin

#define ROW_PORT_0			GPIOE
#define ROW_PIN_0			ROW0_Pin
#define ROW_PORT_1			GPIOE
#define ROW_PIN_1			ROW1_Pin
#define ROW_PORT_2			GPIOE
#define ROW_PIN_2			ROW2_Pin



typedef struct{
      uint8_t id;
      uint8_t modifiers;
      uint8_t key1;
      uint8_t key2;
      uint8_t key3;
      uint8_t key4;
      uint8_t key5;
      uint8_t key6;
  }keyboardHID_t;

typedef struct{
    uint8_t id;
    uint8_t keys;
  }mediaHID_t;

struct mouseHID_t {
      uint8_t buttons;
      int8_t x;
      int8_t y;
      int8_t wheel;
  };

typedef struct{
	keyboardHID_t report;
	uint8_t size;
	uint8_t key_array[6];
} keyboardHIDreport;

typedef struct{
	uint8_t row;
	uint8_t col;
	uint8_t key_code;
} key;

typedef struct sixKeyBufferTag{
	key keys[6];
	uint8_t count;
} sixKeyBuffer;

typedef struct{
	sixKeyBuffer buffer;
	uint8_t send_flag;
} sendBuffer;

extern uint16_t col_pins[KEYBOARD_COLUMNS];
extern uint16_t row_pins[KEYBOARD_COLUMNS];

extern key keypress_buffer[20];
extern uint8_t keypress_buffer_index;
extern sendBuffer keys_to_send;
extern sixKeyBuffer approved_keys;
extern keyboardHIDreport keyboard_HID_report;

void keyboardInit();
void clear_keyboard_HID_report();

#endif /* KEYBOARD_H_ */
