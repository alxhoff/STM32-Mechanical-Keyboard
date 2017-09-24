/*
 * error.h
 *
 *  Created on: Sep 21, 2017
 *      Author: alxhoff
 */

#ifndef ERROR_H_
#define ERROR_H_

typedef enum{
	states_ok		= 0,
	states_init_ok	= 0,
	states_init_err = -1,
	states_add_err	= -2
} states_err_t;

typedef enum{
	shift_ok		= 0,
	shift_init_err	= 1
} shift_err_t;

typedef enum{
	km_ok				= 0,
	km_init_err			= -1,
	km_ID_err			= -2,
	km_table_init_err	= -3
} keymap_err_t;

typedef enum {
	key_ok = 0,
	adc_err = -1,
	process_err = -2,
	key_init_err = -3,
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
} key_err_t;

typedef enum {
	screen_ok		= 0,
	screen_init_err	= -1
} screen_err_t;

typedef enum {
	ENOMEM 			= 1,
	EUSB			= 2,
	EBUFF			= 3,
} err_codes_t;

#endif /* ERROR_H_ */
