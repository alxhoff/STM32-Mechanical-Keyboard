/*
 * keyboard.h
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "stm32f4xx_hal.h"

#include "datatypes.h"
#include "states.h"
#include "keyboard_config.h"

typedef struct key_devices key_devices_t;

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

typedef struct keyboard_HID_data{
	uint8_t process_key_buf;

	keyboardHID_t keyboard_report;
	report_states keyboard_state;

	mediaHID_t media_report;
	report_states media_state;

	send_buffer out_buf;

	six_key_buffer shortlist_keys;

	uint8_t prev_report_len;
	uint8_t prev_keys[6];

	keypress_buffer key_buf;
} keyboard_HID_data_t;



typedef struct keyboard_device{
	uint16_t row_pins[KEYBOARD_ROWS];
	GPIO_TypeDef* row_ports[KEYBOARD_ROWS];

	uint16_t col_pins[KEYBOARD_COLS];
	GPIO_TypeDef* col_ports[KEYBOARD_COLS];
} keyboard_device_t;


extern key key_buf[20];
extern uint8_t keypress_buffer_index;
extern send_buffer keys_to_send;
extern six_key_buffer approved_keys;

key_err_t keyboard_init(key_devices_t* keyboard_devices,
		GPIO_TypeDef* row_ports[KEYBOARD_ROWS], uint16_t row_pins[KEYBOARD_ROWS]);
void clear_keyboard_report(  keyboard_HID_data_t* HID_reports);

#endif /* KEYBOARD_H_ */
