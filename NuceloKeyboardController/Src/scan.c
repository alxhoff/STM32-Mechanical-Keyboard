/*
 * scan.c
 *
 *  Created on: Aug 15, 2017
 *      Author: alxhoff
 */

#include "scan.h"
#include "extern.h"
#include "process.h"

/*
 * functions to be called to initiate a scan of the keyboard matrix
 */
key_err_TypeDef scan_key_matrix(keyboard_HID_data* HID_reports)
{

	//reset keypress buffer
	HID_reports->key_buf.index = 0;

	for(uint8_t row=0;row<KEYBOARD_ROWS;row++){
		//Set current column high so that rows can be read
		HAL_GPIO_WritePin(row_ports[row],row_pins[row], GPIO_PIN_SET);
		for(uint8_t col=0;col<KEYBOARD_COLS;col++){
			if(HAL_GPIO_ReadPin(col_ports[col], col_pins[col])){
				//key is pressed
				HID_reports->key_buf.buffer[HID_reports->key_buf.index].col=col;
				HID_reports->key_buf.buffer[HID_reports->key_buf.index].row=row;
				HID_reports->key_buf.index++;
			}
		}
		HAL_GPIO_WritePin(row_ports[row],row_pins[row], GPIO_PIN_RESET);
		HAL_Delay(1);
	}

	if(HID_reports->key_buf.index == 0)
		return empty_buf;

	return key_ok;
}

key_code scan_get_single_key( keymap_list_t* layer_list )
{
	key_code ret = 0;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	//TODO FLASH STUFF
	while(ret == 0){
		for(uint8_t row=0;row<KEYBOARD_ROWS;row++){
			//Set current column high so that rows can be read
			HAL_GPIO_WritePin(row_ports[row],row_pins[row], GPIO_PIN_SET);
			for(uint8_t col=0;col<KEYBOARD_COLS;col++){
				if(HAL_GPIO_ReadPin(col_ports[col], col_pins[col])){
					ret = process_single_key( layer_list, col, row);
					//display_int_on_screen(col, row);
				}
			}
			HAL_GPIO_WritePin(row_ports[row],row_pins[row], GPIO_PIN_RESET);
			HAL_Delay(1);
		}


	}
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	return ret;
}
