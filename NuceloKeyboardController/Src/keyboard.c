/*
 * keyboard.c
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#include "layers.h"
#include "keyboard.h"
#include "states.h"
#include "datatypes.h"
#include "extern.h"



key_err_TypeDef keyboard_init(key_devices_t* keyboard_devices,
		GPIO_TypeDef* row_ports[KEYBOARD_ROWS], uint16_t row_pins[KEYBOARD_ROWS])
//keyboard_HID_data* HID_reports)
{
	//set up GPIO
//	GPIO_TypeDef* col_ports[] = {COL_PORT_0, COL_PORT_1, COL_PORT_2};
//	uint16_t col_pins[] = {COL_PIN_0, COL_PIN_1, COL_PIN_2};
//	GPIO_TypeDef* row_ports[] ={ROW_PORT_0, ROW_PORT_1, ROW_PORT_2, ROW_PORT_3};
//	uint16_t row_pins[] = {ROW_PIN_0, ROW_PIN_1, ROW_PIN_2, ROW_PIN_3};

//	memcpy(keyboard_devices->keyboard->col_ports, col_ports,
//			sizeof(GPIO_TypeDef*) * KEYBOARD_COLS);
//
//	memcpy(keyboard_devices->keyboard->col_pins, col_pins,
//			sizeof(uint8_t) * KEYBOARD_COLS);

	keyboard_devices->keyboard =
			(keyboard_device_t*)malloc(sizeof(keyboard_device_t));
	if(keyboard_devices->keyboard == NULL)
		return init_err;

	memcpy(keyboard_devices->keyboard->row_ports, row_ports,
				sizeof(GPIO_TypeDef*) * KEYBOARD_ROWS);

	memcpy(keyboard_devices->keyboard->row_pins, row_pins,
				sizeof(uint8_t) * KEYBOARD_ROWS);

	GPIO_InitTypeDef GPIO_InitStruct;

	//INIT COLS - output
//	for(int i=0; i<KEYBOARD_COLS; i++){
//		GPIO_InitStruct.Pin = col_pins[i];
//		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//		GPIO_InitStruct.Pull = GPIO_NOPULL;
//		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//		HAL_GPIO_Init(col_ports[i], &GPIO_InitStruct);
//	}

	//INIT ROWS - input
	for(int i=0; i<KEYBOARD_ROWS; i++){
		GPIO_InitStruct.Pin = row_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(row_ports[i], &GPIO_InitStruct);
	}

	keyboard_devices = (key_devices_t*) malloc(sizeof(key_devices_t));
	if(keyboard_devices == NULL)
		return init_err;

	//HID DATA
	keyboard_devices->keyboard_HID =
			(keyboard_HID_data_t*)malloc(sizeof(keyboard_HID_data_t));
	if(keyboard_devices->keyboard_HID == NULL)
		return init_err;

	keyboard_devices->keyboard_HID->keyboard_report.id = 1;
	keyboard_devices->keyboard_HID->keyboard_state = inactive;
	keyboard_devices->keyboard_HID->media_report.id = 2;
	keyboard_devices->keyboard_HID->media_state = inactive;

	keyboard_devices->keyboard_HID->out_buf.key_buf.count = 0;
	keyboard_devices->keyboard_HID->out_buf.med_buf.count = 0;
	keyboard_devices->keyboard_HID->out_buf.mod_buf = 0;

	for(int i=0; i<6; i++)
		keyboard_devices->keyboard_HID->prev_keys[i] = 0;

	keyboard_devices->keyboard_HID->prev_report_len = 0;
	keyboard_devices->keyboard_HID->key_buf.index=0;
	keyboard_devices->keyboard_HID->out_buf.key_buf.count = 0;
	keyboard_devices->keyboard_HID->out_buf.med_buf.count = 0;

	current_keyboard_state = typing;

	return key_ok;
}
