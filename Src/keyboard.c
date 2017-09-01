/*
 * keyboard.c
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#include "keyboard.h"

GPIO_TypeDef* col_ports[] = {COL_PORT_0, COL_PORT_1, COL_PORT_2, COL_PORT_3};
uint16_t col_pins[] = {COL_PIN_0, COL_PIN_1, COL_PIN_2, COL_PIN_3};
GPIO_TypeDef* row_ports[] ={ROW_PORT_0, ROW_PORT_1, ROW_PORT_2};
uint16_t row_pins[] = {ROW_PIN_0, ROW_PIN_1, ROW_PIN_2};

key keypress_buffer[20] = {0};
u08 keypress_buffer_index;

keyboardHIDreport keyboard_HID_report = {0};

void keyboardInit(){

	GPIO_InitTypeDef GPIO_InitStruct;

	//INIT COLS - output
	for(int i=0; i<KEYBOARD_COLS; i++){
		GPIO_InitStruct.Pin = col_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(col_ports[i], &GPIO_InitStruct);
	}

	//INIT ROWS - input
	for(int i=0; i<KEYBOARD_ROWS; i++){
		GPIO_InitStruct.Pin = row_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(row_ports[i], &GPIO_InitStruct);
	}

	clear_keyboard_HID_report();
}

void clear_keyboard_HID_report(){
	//reset HID
		keyboard_HID_report.report.id = 1;
		keyboard_HID_report.report.key1 = 0;
		keyboard_HID_report.report.key2 = 0;
		keyboard_HID_report.report.key3 = 0;
		keyboard_HID_report.report.key4 = 0;
		keyboard_HID_report.report.key5 = 0;
		keyboard_HID_report.report.key6 = 0;
		keyboard_HID_report.report.modifiers = 0;
}
