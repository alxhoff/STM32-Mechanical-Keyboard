/*
 * scan.c
 *
 *  Created on: Aug 15, 2017
 *      Author: alxhoff
 */

#include "scan.h"
#include "extern.h"

/*
 * functions to be called to initiate a scan of the keyboard matrix
 */
uint8_t scanMatrix(){

	//reset keypress buffer
	keypress_buffer_index = 0;

	for(u08 col=0;col<KEYBOARD_COLS;col++){
		//Set current col low so that rows can be read
		HAL_GPIO_WritePin(col_ports[col],col_pins[col], GPIO_PIN_SET);
		for(u08 row=0;row<KEYBOARD_ROWS;row++){
			if(HAL_GPIO_ReadPin(row_ports[row], row_pins[row])){
				//key is pressed
				keypress_buffer[keypress_buffer_index].col=col;
				keypress_buffer[keypress_buffer_index].row=row;
				keypress_buffer_index++;
			}
		}
		HAL_GPIO_WritePin(col_ports[col], col_pins[col], GPIO_PIN_RESET);
		HAL_Delay(1);
	}

	return 1;
}
