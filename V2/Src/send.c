/*
 * send.c
 *
 *  Created on: 26 Jan 2019
 *      Author: alxhoff
 */
#include "keyboard.h"
#include "buffers.h"
#include "pipes.h"
#include "error.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "lookup.h"

#define INTER_PACKET_DELAY	vTaskDelay(USBD_HID_GetPollingInterval(&hUsbDeviceFS))

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
	const unsigned char 	id; /**< HID report ID for the media report must be 2, as described in
	 the HID descriptor*/
	unsigned char keys; /**< Single media key contained in the media HID report */
} mediaHID_t;

//mouse HID must always be 3
typedef struct mouse_HID{
	const unsigned char 	id;
	unsigned char 			buttons;
	signed char				x;
	signed char				y;
	signed char 			wheel;
  } mouseHID_t;

keyboardHID_t keyboard_report_buf = {.id = 1};
mediaHID_t media_report_buf = {.id = 2};
mouseHID_t mouse_report_buf = {.id = 3};
send_buffer_t send_buf = {0};
static SemaphoreHandle_t processing_lock = NULL;
SemaphoreHandle_t USB_send_lock = NULL;

unsigned char send_get_send_buf(void) {
	unsigned char ret = 0;
	if(queue_packet_to_send)
		if(xSemaphoreTake(processing_lock, (TickType_t) 0) == pdTRUE ){
			ret = xQueueReceive(queue_packet_to_send, &send_buf, (TickType_t) portMAX_DELAY);
			if(ret == pdTRUE)
				return 0;
			goto error;
		}
error:
	xSemaphoreGive(processing_lock);
	return -ENOINIT;
}

unsigned char send_prepare_keyboard(void) {
	memset(&keyboard_report_buf.modifiers, 0, /* clear report without changing id*/
			sizeof(keyboardHID_t) - sizeof(unsigned char));

	memcpy(&keyboard_report_buf.key1,
			&send_buf.key_buf.keys, sizeof(unsigned char) * 6);
	keyboard_report_buf.modifiers = send_buf.mod_buf;

	return 0;
}

unsigned char send_prepare_media(void) {
	media_report_buf.keys = send_buf.med_buf;
	return 0;
}

unsigned char send_prepare_mouse(void)
{
	//TODO
	return 0;
}

void send_blank_keyboard_report(void) {
	keyboardHID_t blanke_report = {.id = 1};
	if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
		while(USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&blanke_report,
				sizeof(keyboardHID_t)) == USBD_FAIL)
			vTaskDelay(1); /* wait for send */ //TODO handle better maybe?

		xSemaphoreGive(USB_send_lock);
	}
}

unsigned char send_keyboard_report(void)
{
	if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
		while(USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&keyboard_report_buf,
				sizeof(keyboardHID_t)) == USBD_FAIL)
			vTaskDelay(1); /* wait for send */ //TODO handle better maybe?

		xSemaphoreGive(USB_send_lock);
	}
	return 0;
}

unsigned char send_media_report(void)
{
	if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			while(USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&media_report_buf,
					sizeof(mediaHID_t)) == USBD_FAIL)
				vTaskDelay(1); /* wait for send */ //TODO handle better maybe?

			xSemaphoreGive(USB_send_lock);
		}
	return 0;
}

unsigned char send_mouse_report(void)
{
	if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
		if(USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&mouse_report_buf,
				sizeof(mouseHID_t))){
			xSemaphoreGive(USB_send_lock);
			return -ESEND;
		}
		xSemaphoreGive(USB_send_lock);
	}
	return 0;
}

unsigned char send_reports(void)
{
	unsigned char ret = 0;

	ret = send_prepare_keyboard();
	if(!ret)
		send_keyboard_report();

//	ret = send_prepare_media();
//	if(!ret)
//		send_media_report();

//	ret = send_prepare_mouse();
//	if(!ret)
//		send_mouse_report();


	xSemaphoreGive(processing_lock);

	return 0;
}

//TODO optimize packet sending to allow for packets of keys with the same modifiers to be send in the
// same packet
void send_string(char* str) {
	static keyboardHID_t tmp_report = {.id = 1};

	while(*str){
		tmp_report.key1 = lookup_get_key(*str);
		tmp_report.modifiers = lookup_get_mod(*str);
		str++;

		if(xSemaphoreTake( USB_send_lock, (TickType_t) portMAX_DELAY) == pdTRUE){
			while(USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&tmp_report,
					sizeof(keyboardHID_t)) == USBD_FAIL)
				vTaskDelay(1); /* wait for send */ //TODO handle better maybe?
			xSemaphoreGive(USB_send_lock);
		}
		vTaskDelay(30);
	}
	send_blank_keyboard_report();
}

//SEND STATE FUNCTIONS
void send_init(void)
{
	processing_lock = xSemaphoreCreateMutex();
	if(!processing_lock)
		return;
	USB_send_lock = xSemaphoreCreateMutex();
	if(!USB_send_lock)
		return;
}

void send_enter(void){
}

void send_run(void){
	if(!send_get_send_buf())
		send_reports();
}

void send_exit(void){
}
