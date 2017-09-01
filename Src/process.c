/*
 * process.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include "process.h"
#include "extern.h"

sendBuffer keys_to_send;
sixKeyBuffer approved_keys;

void processBuffer(){
	//reset buffers
	resetBuffer(&keys_to_send.buffer);
	resetBuffer(&approved_keys);

	//iterate through buffer and translate
	for(int i=0;i<keypress_buffer_index;i++){
		//get character
		keypress_buffer[i].key_code = keymap[0][keypress_buffer[i].col][keypress_buffer[i].row];
		//check if key_code is in last HID report
		for(int j=0;j<keyboard_HID_report.size;j++){
			//if yes put into keys_to_send
			if(keypress_buffer[i].key_code == keyboard_HID_report.key_array[j]){
				keys_to_send.buffer.keys[keys_to_send.buffer.count].key_code = keypress_buffer[i].key_code;
				keys_to_send.buffer.count++;
			}
		}
			//else put into approved_keys
		approved_keys.keys[approved_keys.count].key_code = keypress_buffer[i].key_code;
		approved_keys.count++;
	}
	//fill keys_to_send with keys from approved_keys
	for(int i=keys_to_send.buffer.count; i<keys_to_send.buffer.count+approved_keys.count;i++){
		keys_to_send.buffer.keys[i].key_code = approved_keys.keys[i-keys_to_send.buffer.count].key_code;
	}
}

void resetBuffer(sixKeyBuffer* buffer_to_reset){
	buffer_to_reset->count=0;
	for(int i=0;i<6;i++){
		buffer_to_reset->keys[i].key_code = 0;
	}
}

void createHIDReport(){
	//create HID report
	keyboard_HID_report.report.id = 1;
	keyboard_HID_report.report.key1 = keys_to_send.buffer.keys[0].key_code;
	keyboard_HID_report.key_array[0] = keyboard_HID_report.report.key1;
	keyboard_HID_report.report.key2 = keys_to_send.buffer.keys[1].key_code;
	keyboard_HID_report.key_array[1] = keyboard_HID_report.report.key2;
	keyboard_HID_report.report.key3 = keys_to_send.buffer.keys[2].key_code;
	keyboard_HID_report.key_array[2] = keyboard_HID_report.report.key3;
	keyboard_HID_report.report.key4 = keys_to_send.buffer.keys[3].key_code;
	keyboard_HID_report.key_array[3] = keyboard_HID_report.report.key4;
	keyboard_HID_report.report.key5 = keys_to_send.buffer.keys[4].key_code;
	keyboard_HID_report.key_array[4] = keyboard_HID_report.report.key5;
	keyboard_HID_report.report.key6 = keys_to_send.buffer.keys[5].key_code;
	keyboard_HID_report.key_array[5] = keyboard_HID_report.report.key6;
}

void sendHIDReport(){
	USBD_HID_SendReport(&hUsbDeviceFS, &keyboard_HID_report.report, sizeof(keyboardHID_t));
}
