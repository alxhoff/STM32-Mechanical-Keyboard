/*
 * keyboard.c
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#include "keyboard.h"

GPIO_TypeDef* col_ports[] = {COL_PORT_0, COL_PORT_1, COL_PORT_2};
uint16_t col_pins[] = {COL_PIN_0, COL_PIN_1, COL_PIN_2};
GPIO_TypeDef* row_ports[] ={ROW_PORT_0, ROW_PORT_1, ROW_PORT_2, ROW_PORT_3};
uint16_t row_pins[] = {ROW_PIN_0, ROW_PIN_1, ROW_PIN_2, ROW_PIN_3};

key_err_TypeDef keyboardInit( keyboard_HID_data* HID_reports)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	//INIT COLS - output
	for(int i=0; i<KEYBOARD_ROWS; i++){
		GPIO_InitStruct.Pin = row_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(row_ports[i], &GPIO_InitStruct);
	}

	//INIT ROWS - input
	for(int i=0; i<KEYBOARD_COLS; i++){
		GPIO_InitStruct.Pin = col_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(col_ports[i], &GPIO_InitStruct);
	}

	for(int i=0; i<6; i++)
		HID_reports->prev_keys[i] = 0;

	HID_reports->prev_report_len = 0;
	HID_reports->key_buf.index=0;
	HID_reports->out_buf.key_buf.count = 0;
	HID_reports->out_buf.med_buf.count = 0;

	//set states
	HID_reports->keyboard_state = inactive;
	HID_reports->media_state = inactive;

	return key_ok;
}
