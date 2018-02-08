/**
 * @file scan.c
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Functions used to scan the keyboard for input
 */

#include "keymap.h"
#include "keyboard.h"
#include "scan.h"
#include "extern.h"
#include "lookup.h"
#include "types.h"
#include "shift.h"

int8_t scan_key_matrix(keyboard_device_t* keyboard_dev, keyboard_HID_data_t* HID_reports,
		shift_array_t* shift_array)
{
	//reset keypress buffer
	HID_reports->key_buf.index = 0;

	uint8_t row_mask[shift_array->dev_count];
	for(int i = 0; i<shift_array->dev_count; i++)
		row_mask[i] = 0x00;

	shift_array->set_data(shift_array, row_mask);
	shift_array->output(shift_array, shift_array->dev_count);

	for(uint8_t row=0;row<KEYBOARD_COLS;row++){
		//Set current column high so that rows can be read
		row_mask[row/8] = (1<<(row-((row/8)*8)));
		shift_array->set_byte(shift_array, row/8, row_mask[row/8]);
		shift_array->output(shift_array, shift_array->dev_count);

		for(uint8_t col=0;col<KEYBOARD_ROWS;col++){
			volatile GPIO_PinState tmp = HAL_GPIO_ReadPin(keyboard_dev->col_ports[col], keyboard_dev->col_pins[col]);
 			if(HAL_GPIO_ReadPin(keyboard_dev->col_ports[col], keyboard_dev->col_pins[col])){
				//key is pressed
				HID_reports->key_buf.buffer[HID_reports->key_buf.index].col=col;
				HID_reports->key_buf.buffer[HID_reports->key_buf.index].row=row;
				HID_reports->key_buf.index++;
			}
		}
		row_mask[row/8] = 0;
		shift_array->set_byte(shift_array, 0, 0x00);
		shift_array->output(shift_array, 1);
	}

	if(HID_reports->key_buf.index == 0)
		return -EBUFF;

	return 0;
}

key_code scan_get_single_key( keyboard_device_t* keyboard_dev, keymap_list_t* layer_list )
{
	key_code ret = 0;
	static uint8_t row_mask = 0x00;

	//TODO FLASH STUFF
	while(ret == 0){
		for(uint8_t row=0;row<KEYBOARD_ROWS;row++){
			//Set current column high so that rows can be read
			row_mask = (1<<(7-row));
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, row_mask);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);

			for(uint8_t col=0;col<KEYBOARD_COLS;col++){
				if(HAL_GPIO_ReadPin(keyboard_dev->col_ports[col], keyboard_dev->col_pins[col])){
					ret = process_single_key( layer_list, col, row);
				}
			}
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, 0x00);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);
		}
	}
	vTaskDelay(100);
	return ret;
}

key_code_w_mod_t scan_get_single_key_w_mod( keyboard_device_t* keyboard_dev,
		keymap_list_t* layer_list )
{
	key_code ret = 0;
	static uint8_t shift_modifier = 0;
	static uint8_t row_mask = 0x00;

	key_code_w_mod_t return_pair = {0};

	//TODO FLASH STUFF
	while(return_pair.key_code == 0){
		return_pair.modifier = 0;
		for(uint8_t row=0;row<KEYBOARD_ROWS;row++){
			//Set current column high so that rows can be read
			row_mask = (1<<(7-row));
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, row_mask);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);

			for(uint8_t col=0;col<KEYBOARD_COLS;col++){
				if(HAL_GPIO_ReadPin(keyboard_dev->col_ports[col], keyboard_dev->col_pins[col])){
					ret = process_single_key( layer_list, col, row);
					if(ret == 0xE1 || ret == 0xE5){
						return_pair.modifier = 1;
					}else
						return_pair.key_code = ret;
				}
			}
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, 0x00);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);
		}
	}

	vTaskDelay(100);

	return return_pair;
}

//TODO PROBS NEEDS FIXING VVVV
char* scan_get_single_key_char( keyboard_device_t* keyboard_dev, keymap_list_t* layer_list )
{
	key_code ret = 0;
	static uint8_t shift_modifier = 0;
	static uint8_t row_mask = 0x00;

	//TODO FLASH STUFF
	while(ret == 0){
		shift_modifier = 0;
		for(uint8_t row=0;row<KEYBOARD_ROWS;row++){
			//Set current column high so that rows can be read
			row_mask = (1<<(7-row));
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, row_mask);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);

			for(uint8_t col=0;col<KEYBOARD_COLS;col++){
				if(HAL_GPIO_ReadPin(keyboard_dev->col_ports[col], keyboard_dev->col_pins[col])){
					ret = process_single_key( layer_list, col, row);
					if(ret == 0xE1 || ret == 0xE5){
						shift_modifier = 1;
						ret = 0;
					}
				}
			}
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, 0x00);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);
		}
	}

	vTaskDelay(100);

	if(shift_modifier)
		return lookup_char[(uint8_t)ret].modified;
	else
		return lookup_char[(uint8_t)ret].unmodified;
}

char* scan_get_input_seq( keyboard_device_t* keyboard_dev,
		keymap_list_t* layer_list, char exit_char )
{
	static char* input_str = "";
	static char input_char = 0;
	static uint8_t shift_modifier = 0;
	static char* lookup_str = "";
	static size_t str_size = 0;
	input_str = (char*)malloc(sizeof(char));

	uint8_t finished = 0;
	static uint8_t row_mask = 0x00;

	//debounce stuff
	uint8_t button_input[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	uint8_t button_last_state[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	uint8_t button_cur_state[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	TickType_t button_last_time[KEYBOARD_ROWS][KEYBOARD_COLS] = {0};
	TickType_t debounce_delay = pdMS_TO_TICKS(DEBOUNCE_DELAY);

	keymap_layer_t* current_layer = layer_table_get_current_layer(layer_list);

	while(!finished){
		for(uint8_t row=0;row<KEYBOARD_ROWS;row++){

			row_mask = (1<<(7-row));
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, row_mask);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);

			for(uint8_t col=0;col<KEYBOARD_COLS;col++){

				button_input[row][col] = !HAL_GPIO_ReadPin(keyboard_dev->col_ports[col],
											keyboard_dev->col_pins[col]);

				if(button_input[row][col] != button_last_state[row][col])
					button_last_time[row][col] = xTaskGetTickCount();

				if((xTaskGetTickCount() - button_last_time[row][col]) > debounce_delay){
					if( button_input[row][col] != button_cur_state[row][col]){
						button_cur_state[row][col] = button_input[row][col]; // <---
						if(!button_input[row][col]){
							if(str_size%8 <= 4){
								input_str = (char*)realloc(input_str, sizeof(char) * (str_size + 8));
								if(!input_str) return NULL;
							}
							input_char = current_layer->grid[row][col];
							if(input_char != 0){
//								}
								if(input_char == exit_char){
									finished = 1;
									break;
								}
								//modifier
								if(input_char == 0xE1 || input_char == 0xE5){
									shift_modifier |= (1 << (input_char - 0xE0));
								}else{
									if((shift_modifier & (1<<1)) || (shift_modifier & (1<<5)))
										lookup_str = lookup_char[(uint8_t)input_char].modified;
									else
										lookup_str = lookup_char[(uint8_t)input_char].unmodified;
									strncpy(&input_str[str_size], lookup_str, strlen(lookup_str));
									str_size+=strlen(lookup_str);
								}
								input_char = 0;
							}
						}else{
							input_char = current_layer->grid[row][col];
							if(input_char == 0xE1 || input_char == 0xE5)
									shift_modifier &= ~(1 << (input_char - 0xE0));
							input_char = 0;
						}
					}
				}
				button_last_state[row][col] = button_input[row][col];
			}
			GET_SHIFT_DEVICE->set_byte(GET_SHIFT_DEVICE, 0, 0x00);
			GET_SHIFT_DEVICE->output(GET_SHIFT_DEVICE, 1);
		}
	}
	input_str[str_size++]='\0';

	return realloc(input_str, sizeof(char)*str_size);
}
