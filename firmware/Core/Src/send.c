/*
 * send.c
 *
 *  Created on: 11 Jun 2022
 *      Author: alxhoff
 */

#include "usb_device.h"
#include "usbd_hid.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#include "keyboard.h"
#include "buffers.h"

/**
 * @typedef keyboardHID_t
 * @brief Typedef of keyboardHID
 * */
/**
 * @struct keyboardHID
 * @brief Keyboard HID report
 *
 * The keyboard HID report is the USB HID report sent to handle all keyboard press
 * events, its structure is defined by the HID descriptor.
 *
 * */
typedef struct keyboardHID {
	const unsigned char id; /**< HID report ID for the keyboard report must always be 1, as described
	 in the HID descriptor*/
	unsigned char modifiers; /**< Byte that send key modifiers. See HIDClassCommon.h for bit details*/
	/** @defgroup HID_keys HID keys
	 * @brief Each key byte represents a keypress to be sent via USB
	 *  @{
	 */
	unsigned char key1;
	unsigned char key2;
	unsigned char key3;
	unsigned char key4;
	unsigned char key5;
	unsigned char key6;
/** @} */ // end of HID_keys
} keyboardHID_t;

/**
 * @typedef mediaHID_t
 * @brief Typedef of mediaHIDg
 * */
/**
 * @struct mediaHIDg
 * @brief Media HID report
 *
 * The media HID report is the USB HID report sent to handle all media key press
 * events, its structure is defined by the HID descriptor.
 *
 * */
typedef struct mediaHID {
	const unsigned char id; /**< HID report ID for the media report must be 2, as described in
	 the HID descriptor*/
	unsigned char keys; /**< Single media key contained in the media HID report */
} mediaHID_t;

//mouse HID must always be 3
typedef struct mouse_HID {
	const unsigned char id;
	unsigned char buttons;
	signed char x;
	signed char y;
	signed char wheel;
} mouseHID_t;

TaskHandle_t send_task_handle;
SemaphoreHandle_t send_buffer_lock;
send_buffer_t send_buffer;

unsigned char sendGetNewBuffer() {
	return xQueueReceive(keyboard_send_buffer_queue,
			&keyboard_send_buffer_queue, (TickType_t) 0);
}

keyboardHID_t sendPrepareSixKeyBuffer() {
	keyboardHID_t hid_buff = { .id = 1 };
	unsigned char *key_buff = &hid_buff.key1;

	hid_buff.modifiers = send_buffer.mod_buff;

	//TODO check this
	// While send_buff has keys and key_buff is still pointing to one of the 6 output keys
	for (; send_buffer.key_buff.count && key_buff <= &(hid_buff.key6);
			send_buffer.key_buff.count--) {
		*key_buff = BUFFER_GET_END(send_buffer.key_buff);
		key_buff += sizeof(unsigned char);
	}

	return hid_buff;
}

void sendSendSixKeyBuffer(keyboardHID_t hid_buff) {
	while (USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &hid_buff,
			sizeof(keyboardHID_t)) == USBD_FAIL)
		vTaskDelay(1);
}

void sendTask(void *arguments) {

	six_key_buffer_t tmp_buff = { 0 };

	while (1) {
		if (keyboard_send_buffer_queue)
			if (xSemaphoreTake(send_buffer_lock, (TickType_t) 0) == pdTRUE) {
				if (sendGetNewBuffer()) // If we have a buffer
					while (send_buffer.key_buff.count || send_buffer.med_buff
							|| send_buffer.mod_buff) {
						sendSendSixKeyBuffer(sendPrepareSixKeyBuffer());

						// Clear buffer for next itteration
						memset(&tmp_buff, 0, sizeof(six_key_buffer_t));
					}

				xSemaphoreGive(send_buffer_lock);
			}
	}
}

void sendInit() {
	send_buffer_lock = xSemaphoreCreateMutex();

	xTaskCreate(sendTask, "send_task", 128, NULL, 1, &send_task_handle);

}
