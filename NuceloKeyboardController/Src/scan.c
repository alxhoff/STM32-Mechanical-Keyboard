/*
 * scan.c
 *
 *  Created on: Aug 15, 2017
 *      Author: alxhoff
 */

#include "scan.h"
#include "extern.h"
#include "process.h"
#include "lookup.h"

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
	vTaskDelay(100);
	return ret;
}

char* scan_get_input_seq( keymap_list_t* list )
{
	static char* input_str = "";
	static char input_char = 0;
	static char* lookup_str = "";
	static size_t str_size = 0;
	input_str = (char*)malloc(sizeof(char));

	uint8_t finished = 0;

	//debounce stuff
	uint8_t button_input[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	uint8_t button_last_state[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	uint8_t button_cur_state[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	TickType_t button_last_time[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	TickType_t debounce_delay = pdMS_TO_TICKS(DEBOUNCE_DELAY);

	keymap_layer* current_layer = layer_table_get_current_layer(list);

	while(!finished){
		//FOR
		for(uint8_t row=0;row<KEYBOARD_ROWS;row++){
			HAL_GPIO_WritePin(row_ports[row],row_pins[row], GPIO_PIN_SET);
			for(uint8_t col=0;col<KEYBOARD_COLS;col++){

				button_input[row][col] = !HAL_GPIO_ReadPin(col_ports[col], col_pins[col]);

				if(button_input[row][col] != button_last_state[row][col])
					button_last_time[row][col] = xTaskGetTickCount();

				if((xTaskGetTickCount() - button_last_time[row][col]) > debounce_delay){
					if( button_input[row][col] != button_cur_state[row][col]){
						button_cur_state[row][col] = button_input[row][col]; // <---
						if(!button_input[row][col]){
							//push
							if(str_size%8 <= 4){
								input_str = (char*)realloc(input_str, sizeof(char) * (str_size + 8));
								if(!input_str) return NULL;
							}
							input_char = current_layer->grid[row][col];
							if(input_char == KEY(MACRO_S)){
								finished = 1;
								break;
							}
							if(input_char != 0)
								lookup_str = lookup_char[input_char].unmodified;
								strncpy(&input_str[str_size], lookup_str, strlen(lookup_str));
								str_size+=strlen(lookup_str);
//								input_str[str_size]=input_char;
							input_char = 0;


						}else{
							;
							//release
						}
					}
				}
				button_last_state[row][col] = button_input[row][col];
			}
			HAL_GPIO_WritePin(row_ports[row],row_pins[row], GPIO_PIN_RESET);
			HAL_Delay(1);
		}
	}
	input_str[str_size++]='\0';

	return realloc(input_str, sizeof(char)*str_size);
}
