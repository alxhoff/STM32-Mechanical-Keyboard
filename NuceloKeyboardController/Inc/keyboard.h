/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include "main.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"

//KEYBOARD SIZE
#define KEYBOARD_ROWS 4
#define KEYBOARD_COLS 3

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


#define ROW_PORT_0			GPIOF
#define ROW_PIN_0			ROW0_Pin
#define ROW_PORT_1			GPIOF
#define ROW_PIN_1			ROW1_Pin
#define ROW_PORT_2			GPIOF
#define ROW_PIN_2			ROW2_Pin
#define ROW_PORT_3			GPIOF
#define ROW_PIN_3			ROW3_Pin

typedef struct keyboardHID{
      uint8_t id;
      uint8_t modifiers;
      uint8_t key1;
      uint8_t key2;
      uint8_t key3;
      uint8_t key4;
      uint8_t key5;
      uint8_t key6;
  }keyboardHID_t;

typedef struct mediaHIDg{
    uint8_t id;
    uint8_t keys;
  } mediaHID_t;

typedef struct{
	uint8_t row;
	uint8_t col;
	uint8_t key_code;
} key;

typedef struct{
	key buffer[20];
	uint8_t index;
} keypress_buffer;

typedef struct{
	key keys[6];
	uint8_t count;
} six_key_buffer;

typedef struct{
	key key;
	uint8_t count;
} one_key_buffer;

typedef struct{
	six_key_buffer key_buf;
	uint8_t mod_buf;
	one_key_buffer med_buf;
	uint8_t send_flag;
} send_buffer;

typedef enum{
	inactive,
	active,
	clearing
} report_states;

typedef struct keyboard_HID_data{
	uint8_t process_key_buf;

	keyboardHID_t keyboard;
	report_states keyboard_state;

	mediaHID_t media;
	report_states media_state;

	send_buffer out_buf;

	six_key_buffer shortlist_keys;

	uint8_t prev_report_len;
	uint8_t prev_keys[6];

	keypress_buffer key_buf;
} keyboard_HID_data_t;



typedef enum{
	keyboard,
	media,
	mouse
} report_type;

typedef enum key_err_TypeDef{
	key_ok = 0,
	adc_err = -1,
	process_err = -2,
	init_err = -3,
	send_err = -4,
	empty_buf = -5,
	no_flags = -6,
	reset_err = -7,
	no_mouse_mov = -8,
	mouse_ok = -9,
	key_enter_set = -10,
	key_layer_set = -11,
	key_macro_run = -12,
	key_macro_set = -13
} key_err_TypeDef;

typedef struct keyboard_device{
	uint16_t row_pins[KEYBOARD_ROWS];
	GPIO_TypeDef* row_ports[KEYBOARD_ROWS];

	uint16_t col_pins[KEYBOARD_COLS];
	GPIO_TypeDef* col_ports[KEYBOARD_COLS];
} keyboard_device_t;


extern uint16_t col_pins[KEYBOARD_COLS];
extern uint16_t row_pins[KEYBOARD_ROWS];

extern key key_buf[20];
extern uint8_t keypress_buffer_index;
extern send_buffer keys_to_send;
extern six_key_buffer approved_keys;

key_err_TypeDef keyboard_init( keyboard_HID_data_t* HID_reports );
void clear_keyboard_report(  keyboard_HID_data_t* HID_reports);

#endif /* KEYBOARD_H_ */
