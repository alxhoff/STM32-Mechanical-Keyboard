/*
 * scan.h
 *
 *  Created on: Aug 15, 2017
 *      Author: alxhoff
 */

#ifndef SCAN_H_
#define SCAN_H_

#define DEBOUNCE_DELAY 20

#include "error.h"
#include "keyboard.h"
#include "types.h"

int8_t scan_key_matrix(keyboard_device_t* keyboard_dev,
		keyboard_HID_data_t* HID_reports, shift_array_t* shift_array);
key_code scan_get_single_key( keyboard_device_t* keyboard_dev, keymap_list_t* layer_list );
char* scan_get_input_seq( keyboard_device_t* keyboard_dev,
		keymap_list_t* list, char exit_char );
#endif /* SCAN_H_ */
