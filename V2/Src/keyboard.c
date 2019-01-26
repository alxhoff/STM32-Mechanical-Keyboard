/**
 * @file keyboard.c
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Data types and functions for performing classical keyboard functions
 */

#include "error.h"
#include "config.h"
#include "keyboard.h"
#include "keyboard_private.h"
#include "keymap.h"
#include "cmsis_os.h"
#include "SN54HC595.h"
#include "lookup.h"

#include <stdlib.h>
#include <string.h>

QueueHandle_t queue_packet_to_send;

keyboard_device_t keyboard_dev = { .row_ports = { ROW_PORT_0, ROW_PORT_1,
		ROW_PORT_2, ROW_PORT_3, ROW_PORT_4 }, .row_pins = { ROW_PIN_0,
		ROW_PIN_1, ROW_PIN_2, ROW_PIN_3, ROW_PIN_4 }, .keyboard_report_ID = 1,
		.media_report_ID = 2, };

void keyboard_init_status_LEDS(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure GPIO pin : PA15 */
	GPIO_InitStruct.Pin = CAPS_STATUS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CAPS_STATUS_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin : PA15 */
	GPIO_InitStruct.Pin = CLI_STATUS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CLI_STATUS_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin : PA15 */
	GPIO_InitStruct.Pin = FUNC_STATUS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(FUNC_STATUS_PORT, &GPIO_InitStruct);
}

void keyboard_init_row_inputs(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	//INIT ROWS - input
	for (unsigned char i = 0; i < KEYBOARD_ROWS; i++) {
		GPIO_InitStruct.Pin = keyboard_dev.row_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(keyboard_dev.row_ports[i], &GPIO_InitStruct);
	}
}

void keyboard_init(void) {
	keyboard_init_status_LEDS();
	keyboard_init_row_inputs();

	queue_packet_to_send = xQueueCreate(20, sizeof(send_buffer_t));
//	//HID DATA
//	keyboard_devices->keyboard_HID =
//			(keyboard_HID_data_t*)calloc(1, sizeof(keyboard_HID_data_t));
//	if(keyboard_devices->keyboard_HID == NULL)
//		return key_init_err;
//
//	keyboard_devices->keyboard_HID->keyboard_report.id = 1;
//	keyboard_devices->keyboard_HID->keyboard_state = inactive;
//	keyboard_devices->keyboard_HID->media_report.id = 2;
//	keyboard_devices->keyboard_HID->media_state = inactive;
//
//	keyboard_devices->keyboard_HID->out_buf.key_buf.count = 0;
//	keyboard_devices->keyboard_HID->out_buf.med_buf.count = 0;
//	keyboard_devices->keyboard_HID->out_buf.mod_buf = 0;
//
//	for(int i=0; i<6; i++)
//		keyboard_devices->keyboard_HID->prev_keys[i] = 0;
//
//	keyboard_devices->keyboard_HID->prev_report_len = 0;
//	keyboard_devices->keyboard_HID->key_buf.index=0;
//	keyboard_devices->keyboard_HID->out_buf.key_buf.count = 0;
//	keyboard_devices->keyboard_HID->out_buf.med_buf.count = 0;
//
//	current_keyboard_state = typing;

//status LEDs

}

unsigned char keyboard_read_row(unsigned char row) {
	return HAL_GPIO_ReadPin(keyboard_dev.row_ports[row],
			keyboard_dev.row_pins[row]);
}

void keyboard_scan_buff_reset(void) {
	keyboard_dev.scan_buf.count = 0;
}

void keyboard_scan_buff_add(unsigned char col, unsigned char row) {
	keyboard_dev.scan_buf.buf[keyboard_dev.scan_buf.count].col = col;
	keyboard_dev.scan_buf.buf[keyboard_dev.scan_buf.count].row = row;
	keyboard_dev.scan_buf.count++;
}

unsigned char keyboard_scan_buf_length(void) {
	return keyboard_dev.scan_buf.count;
}

unsigned char keyboard_scan_matrix(void) {
	//TODO check if VVV is needed. Maybe already handled
	// in process_key_buf
	keyboard_scan_buff_reset();

//	static unsigned char row_mask[SHIFT_DEVICES] = {0};
	static unsigned short row_mask = { 0 };
	unsigned char ret = 0;

	for (unsigned char col = 0; col < KEYBOARD_COLS; col++) {
		//Set current column high so that rows can be read
//		row_mask[row/8] = (1<<(row-((row/8)*8)));
		row_mask = (1 << col);

		ret = SN54HC595_out_bytes((unsigned char *)&row_mask, SHIFT_DEVICES);
		if (ret)
			return -EAGAIN;
		vTaskDelay(1);	//wait for shift register

		for (unsigned char row = 0; row < KEYBOARD_ROWS; row++) /* test each row */
			if (keyboard_read_row(row)) /*key is pressed */
				keyboard_scan_buff_add(KEYBOARD_COLS - col - 1, row); //TODO remove - requirement

		//TODO is this necessary?
//		row_mask = 0;
		SN54HC595_clear();
	}

	if (!keyboard_scan_buf_length())
		return -ENOENT;

	return 0;
}

unsigned char is_in_prev_buf(unsigned char sc) {
	static unsigned char i = 0;
	for (; i < keyboard_dev.prev_buf.count; i++)
		if (sc == keyboard_dev.prev_buf.keys[i])
			return 1;
	return 0;
}

// sort all standard keys: normal, mod and media into buffer
//returns -1 if 6 normals keys have been processed
void keyboard_sort_scaned_key(send_buffer_t *send_buf, key_buffer_t *buf_new,
		unsigned char key) {
	if (key >= 0xE8 && key <= 0xEF) /* media */
	{
		send_buf->med_buf = key;
		return;
	}

	if (key >= 0xE0 && key <= 0xE7) /* modifier */
	{
		send_buf->mod_buf |= (1 << (key - 0xE0));
		return;
	}

	//sort key into send buffer or new keys buf
	if (is_in_prev_buf(key))
		send_buf->key_buf.keys[send_buf->key_buf.count++] = key;
	else
		buf_new->buf[buf_new->count++] = key;
}

unsigned char keyboard_process_scan_buf(void) {
	static unsigned char ret = 0;
	volatile static unsigned char tmp_sc;
	unsigned char i = 0;
	static key_buffer_t sc_new = { 0 };

	send_buffer_t *buf = calloc(1, sizeof(send_buffer_t));
	if (!buf)
		return -ENOMEM;

	while (keyboard_dev.scan_buf.count) {
		tmp_sc = keymap_get_key(keyboard_dev.scan_buf.buf[i].row,
				keyboard_dev.scan_buf.buf[i].col);
		/* State change */
		ret = lookup_state_change_key(tmp_sc);
		if (ret) { /* if state change key */
			//TODO HANDLE STATE CHANGE
			keyboard_dev.scan_buf.count = 0; /* clear buffer */
			return 0;
		}

		/* Toggle key */
		ret = lookup_toggle_key(tmp_sc);
		if (ret) {
			//TODO handle toggle press
		}

		/* The rest */
		keyboard_sort_scaned_key(buf, &sc_new, tmp_sc);

		keyboard_dev.scan_buf.count--;
		i++;
	}
	////TODO scan buf count should be zero

	while (buf->key_buf.count < 6 && sc_new.count) /* Fill buf with new keys */
	{
		buf->key_buf.keys[buf->key_buf.count] = sc_new.buf[sc_new.count - 1];
		buf->key_buf.count++;
		sc_new.count--;
	}
	if (!queue_packet_to_send)
		return -ENOINIT;

	if(buf->key_buf.count || buf->med_buf)
		xQueueSend(queue_packet_to_send, buf, portMAX_DELAY);

	memcpy(&keyboard_dev.prev_buf, &buf->key_buf, /* Save for next frame */
	sizeof(six_key_buffer_t));

	return 0;
}

//
//unsigned char process_keyboard_flags ( keyboard_HID_data_t* data )
//{
//	if(data->keyboard_state == active){
//		keyboard_prepare_report(data);
//		send_keyboard_report(data, keyboard);
//		data->keyboard_state = clearing;
//	}else if( data->keyboard_state == clearing){
//		send_keyboard_report(data, keyboard);
//		data->keyboard_state = inactive;
//	}
//	if(data->media_state == active){
//		media_prepare_report(data);
//		send_keyboard_report(data, media);
//		data->media_state = clearing;
//	}else if( data->media_state == clearing){
//		send_keyboard_report(data, media);
//		data->media_state = inactive;
//	}
//	return 0;
//}
//
//unsigned char process_single_key(unsigned char col, unsigned char row )
//{
//	unsigned char ret = 0;
////	ret = layer_list->layer_head->grid[row][col];
//	if(ret)
//		return ret;
//	else
//		return 0;
//}
//
//void clear_keyboard_report(  keyboard_HID_data_t* data )
//{
//	if(data->keyboard_state == clearing || data->keyboard_state == active){
//		data->keyboard_report.key1 = 0;
//		data->keyboard_report.key2 = 0;
//		data->keyboard_report.key3 = 0;
//		data->keyboard_report.key4 = 0;
//		data->keyboard_report.key5 = 0;
//		data->keyboard_report.key6 = 0;
//		data->keyboard_report.modifiers = 0;
//	}
//	if(data->keyboard_state == clearing || data->keyboard_state == active)
//		data->media_report.keys = 0;
//}
//
//void display_int_on_screen(unsigned char col, unsigned char row)
//{
//	static char col_str[10];
//	static char row_str[10];
//	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//
//	sprintf(col_str, "Col: %d", col);
//	sprintf(row_str, "Row: %d", row);
//
//	ssd1306_Fill(White);
//	ssd1306_SetCursor(23,10);
////	ssd1306_WriteString("here",Font_11x18,Black);
//
//	ssd1306_WriteString(&col_str,Font_11x18,Black);
//	ssd1306_SetCursor(23,30);
//	ssd1306_WriteString(&row_str,Font_11x18,Black);
//	ssd1306_UpdateScreen();
//}
