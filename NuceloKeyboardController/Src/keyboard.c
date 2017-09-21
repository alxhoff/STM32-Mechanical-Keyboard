/*
 * keyboard.c
 *
 *  Created on: Aug 14, 2017
 *      Author: alxhoff
 */

#include "keyboard.h"
#include "extern.h"
#include "layers.h"
#include "types.h"
#include "states.h"
#include "ssd1306.h"

key_err_t keyboard_init(key_devices_t* keyboard_devices,
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

key_err_t process_key_buf(keyboard_HID_data_t* data, keymap_list_t* layer_list)
{
	if(data->key_buf.index == 0){
		return empty_buf;
	}

	//reset buffers
	data->out_buf.key_buf.count = 0;
	data->out_buf.med_buf.count = 0;
	data->shortlist_keys.count = 0;

	//clear modifiers
	data->out_buf.mod_buf = 0x00;

	if(current_keyboard_state == layer_set)
		goto set_layer;

	if(current_keyboard_state == macro_run)
		goto macro_run;

	if(current_keyboard_state == macro_set)
		goto macro_set;

	//get current layer
	keymap_layer_t* current_layer = layer_table_get_current_layer(layer_list);

	//TODO PUT INTO STATE FUNCTIONS
	//iterate through buffer and translate
	for(int i=0;i<data->key_buf.index;i++){
		//TODO capslock
		//get character
		data->key_buf.buffer[i].key_code =
				current_layer->grid[data->key_buf.buffer[i].row][data->key_buf.buffer[i].col];

		//LAYER MODIFIER
		if(data->key_buf.buffer[i].key_code == HID_KEYBOARD_SC_LAYER_FUNCTION)
			goto state_change_layer;

		//MACRO_RUN
		if(data->key_buf.buffer[i].key_code == HID_KEYBOARD_SC_MACRO_RUN_FUNCTION)
			goto state_change_macro_run;

		//MACRO_SET
		if(data->key_buf.buffer[i].key_code == HID_KEYBOARD_SC_MACRO_SET_FUNCTION)
			goto state_change_macro_set;

		//MODIFIER
		if(data->key_buf.buffer[i].key_code >= 0xE0 && data->key_buf.buffer[i].key_code <= 0xE7){
			data->keyboard_state = active;
			data->out_buf.mod_buf |= (1 << (data->key_buf.buffer[i].key_code - 0xE0));
		}
		//MEIDA
		else if(data->key_buf.buffer[i].key_code >= 0xE8 && data->key_buf.buffer[i].key_code <= 0xEF){
			//media
			data->media_state = active;
			//TODO intelligent media
			if(data->out_buf.med_buf.count <= 1){
				data->out_buf.med_buf.key.key_code =
						data->key_buf.buffer[i].key_code;
				data->out_buf.med_buf.count++;
			}
		//OTHER KEY PRESSES
		}else{
			data->keyboard_state = active;
			//check if key_code is in last HID report
			for(int j=0;j<data->prev_report_len;j++){
				//if yes put into data->out_buf
				if(data->key_buf.buffer[i].key_code == data->prev_keys[j]){

					data->out_buf.key_buf.keys[data->out_buf.key_buf.count].key_code =
							data->key_buf.buffer[i].key_code;
					data->out_buf.key_buf.count++;

					break;
				}
			}
				//else put into data->shortlist_keys
			data->shortlist_keys.keys[data->shortlist_keys.count].key_code =
							data->key_buf.buffer[i].key_code;
			data->shortlist_keys.count++;
		}
	}

	//fill data->out_buf with keys from data->shortlist_keys
	if(data->keyboard_state == active){
		int out_buf_count = data->out_buf.key_buf.count;

		for(int i=data->out_buf.key_buf.count;
				i<out_buf_count + data->shortlist_keys.count;i++){
			data->out_buf.key_buf.keys[i].key_code =
					data->shortlist_keys.keys[i - out_buf_count].key_code;
			data->out_buf.key_buf.count++;
			if(i==5)
				break;
		}
	}

	//prepare for sending
	data->prev_report_len = data->out_buf.key_buf.count;

	return key_ok;
	state_change_layer: state_enter_layer_set();
	set_layer: state_layer_set( layer_list );
	return key_layer_set;
	state_change_macro_run: state_enter_macro_run();
	macro_run: state_macro_run( layer_list );
	return key_macro_run;
	state_change_macro_set: state_enter_macro_set();
	macro_set: state_macro_set( layer_list );
	return key_macro_set;
}

signed int reset_buffer(six_key_buffer* buffer_to_reset)
{
	buffer_to_reset->count=0;
	for(int i=0;i<6;i++){
		buffer_to_reset->keys[i].key_code = 0;
	}
	return 0;
}

void clear_keyboard_report(  keyboard_HID_data_t* data )
{
	if(data->keyboard_state == clearing || data->keyboard_state == active){
		data->keyboard_report.key1 = 0;
		data->keyboard_report.key2 = 0;
		data->keyboard_report.key3 = 0;
		data->keyboard_report.key4 = 0;
		data->keyboard_report.key5 = 0;
		data->keyboard_report.key6 = 0;
		data->keyboard_report.modifiers = 0;
	}
	if(data->keyboard_state == clearing || data->keyboard_state == active)
		data->media_report.keys = 0;
}

//TODO int -> uint8_t for loops
key_err_t keyboard_prepare_report( keyboard_HID_data_t* data )
{
	for(int i = 0; i < data->out_buf.key_buf.count; i++){
		*(&data->keyboard_report.key1 + i * sizeof(uint8_t)) = data->out_buf.key_buf.keys[i].key_code;
		data->prev_keys[i] = *(&data->keyboard_report.key1 + i * sizeof(uint8_t));
	}
	for(int i = data->out_buf.key_buf.count; i<6 ; i++){
		*(&data->keyboard_report.key1 + i * sizeof(uint8_t)) = 0x00;
		data->prev_keys[i] = *(&data->keyboard_report.key1 + i * sizeof(uint8_t));
	}

	data->keyboard_report.modifiers = data->out_buf.mod_buf;

	return key_ok;
}

key_err_t media_prepare_report( keyboard_HID_data_t* data )
{
	for(int i = 0; i < data->out_buf.med_buf.count; i++)
		data->media_report.keys = data->out_buf.med_buf.key.key_code;

	return key_ok;
}

key_err_t send_keyboard_report( keyboard_HID_data_t* data, report_type type )
{
	switch(type){
	case keyboard:
		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			USBD_HID_SendReport(&hUsbDeviceFS, &data->keyboard_report, sizeof(keyboardHID_t));
			data->keyboard_state = clearing;
			xSemaphoreGive(USB_send_lock);
		}
		return key_ok;
		break;
	case media:
		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			USBD_HID_SendReport(&hUsbDeviceFS, &data->media_report, sizeof(mediaHID_t));
			data->media_state = clearing;
			xSemaphoreGive(USB_send_lock);
		}
		return key_ok;
		break;
	case mouse:
	default:
		return send_err;
	}
	return key_ok;
}

key_err_t process_keyboard_flags ( keyboard_HID_data_t* data )
{
	if(data->keyboard_state == active){
		keyboard_prepare_report(data);
		send_keyboard_report(data, keyboard);
		data->keyboard_state = clearing;
	}else if( data->keyboard_state == clearing){
		send_keyboard_report(data, keyboard);
		data->keyboard_state = inactive;
	}

	if(data->media_state == active){
		media_prepare_report(data);
		send_keyboard_report(data, media);
		data->media_state = clearing;
	}else if( data->media_state == clearing){
		send_keyboard_report(data, media);
		data->media_state = inactive;
	}

	return key_ok;
}

uint8_t process_single_key( keymap_list_t* layer_list, uint8_t col, uint8_t row )
{
	uint8_t ret = 0;
	ret = layer_list->layer_head->grid[row][col];
	if(ret)
		return ret;
	else
		return 0;
}

void display_int_on_screen(uint8_t col, uint8_t row)
{
	static char col_str[10];
	static char row_str[10];
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);

	sprintf(col_str, "Col: %d", col);
	sprintf(row_str, "Row: %d", row);

	ssd1306_Fill(White);
	ssd1306_SetCursor(23,10);
//	ssd1306_WriteString("here",Font_11x18,Black);

	ssd1306_WriteString(&col_str,Font_11x18,Black);
	ssd1306_SetCursor(23,30);
	ssd1306_WriteString(&row_str,Font_11x18,Black);
	ssd1306_UpdateScreen();
}
