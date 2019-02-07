/**
 * @file keyboard.c
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Data types and functions for performing classical keyboard functions
 */

#include "error.h"
#include "config.h"
#include "keyboard.h"
#include "buffers.h"
#include "keymap.h"
#include "cmsis_os.h"
#include "SN54HC595.h"
#include "lookup.h"
#include "LEDs.h"

#include <stdlib.h>
#include <string.h>

#include "buffers.h"

/**
 * @typedef scaned_key_t
 * @brief Typedef of scaned_key
 * */
/**
 * @struct scaned_key
 * @brief Represents a single keyboard key
 *
 * The key structure represents a key at a given moment in time by representing
 * its row and column as well as its current key_code. Does not account for layers
 * as it stores one static key_code which can be dynamically changed.
 *
 * */
typedef struct scaned_key {
	unsigned char row; /**< Key's row */
	unsigned char col; /**< Key's col */
} scaned_key_t;

/**
 * @typedef scan_buffer_t
 * @brief Typedef of scan_buffer
 * */
/**
 * @struct scan_buffer
 * @brief Keypress input buffer used to store keystroke input from keyboard.
 *
 * When the keyboard is initially scanned, all keys that were found to be pressed are
 * stored in the keypress buffer before being processed. The key presses are saved as
 * row and col values, not key_codes.
 *
 * */
typedef struct scan_buffer {
	scaned_key_t buf[SCAN_KEY_BUFFER_LENGTH]; /**< Key buffer */
	unsigned char count; /**< Stores the amount of keys currently within the buffer*/
} scan_buffer_t;

typedef struct key_buffer {
	unsigned char buf[SCAN_KEY_BUFFER_LENGTH];
	unsigned char count;
} key_buffer_t;

typedef struct toggle_keys {
	unsigned char caps;
	unsigned char CLI;
	unsigned char func;

	unsigned char changed;
} toggle_keys_t;

/**
 * @typedef keyboard_device_t
 * @brief Typedef of keyboard_device
 * */
/**
 * @struct keyboard_device
 * @brief Stores the GPIO interface information
 *
 * Details the GPIO pins and ports used to access the rows
 * and columns of the keyboard.
 *
 * */
typedef struct keyboard_device {
	uint16_t row_pins[KEYBOARD_ROWS]; /**< GPIO pins used by the keyboard rows */
	GPIO_TypeDef* row_ports[KEYBOARD_ROWS]; /**< GPIO ports used by the keyboard rows */

	const unsigned char keyboard_report_ID;
	const unsigned char media_report_ID;

	//TODO lock this to prepare for multicore
	SemaphoreHandle_t buf_lock;
	scan_buffer_t scan_buf; /**< Buffer where scanned keys are stored before processing */
	six_key_buffer_t prev_buf;

	toggle_keys_t toggles;
} keyboard_device_t;

QueueHandle_t queue_packet_to_send;

keyboard_device_t keyboard_dev = { .row_ports = { ROW_PORT_0, ROW_PORT_1,
ROW_PORT_2, ROW_PORT_3, ROW_PORT_4 }, .row_pins = { ROW_PIN_0,
ROW_PIN_1, ROW_PIN_2, ROW_PIN_3, ROW_PIN_4 }, .keyboard_report_ID = 1,
		.media_report_ID = 2, };

static void keyboard_init_status_LEDS(void) {
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

static void keyboard_init_row_inputs(void) {
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
	keyboard_dev.buf_lock = xSemaphoreCreateMutex();
}

static unsigned char keyboard_read_row(unsigned char row) {
	return HAL_GPIO_ReadPin(keyboard_dev.row_ports[row],
			keyboard_dev.row_pins[row]);
}

static void keyboard_scan_buff_add(unsigned char col, unsigned char row) {
	keyboard_dev.scan_buf.buf[keyboard_dev.scan_buf.count].col = col;
	keyboard_dev.scan_buf.buf[keyboard_dev.scan_buf.count].row = row;
	keyboard_dev.scan_buf.count++;
}

unsigned char keyboard_scan_matrix(void) {

	static unsigned short row_mask = { 0 };
	unsigned char ret = 0;

	if ( xSemaphoreTake(keyboard_dev.buf_lock, (TickType_t) 0) == pdFALSE)
		return -EAGAIN;

	for (unsigned char col = 0; col < KEYBOARD_COLS; col++) { /* Set each col high and test rows*/
		row_mask = (1 << col);

		ret = SN54HC595_out_bytes((unsigned char *) &row_mask, SHIFT_DEVICES);
		vTaskDelay(1);
		if (ret)
			goto error;

		for (unsigned char row = 0; row < KEYBOARD_ROWS; row++) /* test each row */
			if (keyboard_read_row(row)) /*key is pressed */
				keyboard_scan_buff_add(KEYBOARD_COLS - col - 1, row); //TODO remove - requirement

//		SN54HC595_clear();
	}

	xSemaphoreGive(keyboard_dev.buf_lock);
	return 0;

	error:
	xSemaphoreGive(keyboard_dev.buf_lock);
	return -EAGAIN;
}

static unsigned char is_in_prev_buf(unsigned char sc) {
	static unsigned char i = 0;
	for (; i < keyboard_dev.prev_buf.count; i++)
		if (sc == keyboard_dev.prev_buf.keys[i])
			return 1;
	return 0;
}

// sort all standard keys: normal, mod and media into buffer
//returns -1 if 6 normals keys have been processed
static void keyboard_sort_scaned_key(send_buffer_t *send_buf,
		key_buffer_t *buf_new, unsigned char key) {
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

void keyboard_toggle_caps(void) {
	keyboard_dev.toggles.caps ^= 1;
}

void keyboard_toggle_CLI(void) {
	keyboard_dev.toggles.CLI ^= 1;
}

void keyboard_toggle_func(void) {
	keyboard_dev.toggles.func ^= 1;
}

unsigned char keyboard_process_scan_buf(void) {
	static unsigned char ret = 0;
	volatile static unsigned char tmp_sc;
	unsigned char i = 0;
	static key_buffer_t sc_new = { 0 };

	send_buffer_t *buf = calloc(1, sizeof(send_buffer_t));
	if (!buf)
		return -ENOMEM;

	if (xSemaphoreTake(keyboard_dev.buf_lock, (TickType_t) 0) == pdTRUE) {

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
			//TODO make more efficient
			ret = lookup_toggle_key(tmp_sc);
			if (ret) {
				switch (ret) {
				case 1:
					keyboard_toggle_caps();
					keyboard_dev.toggles.changed = 1;
					break;
				default:
					break;
				}
			}

			/* The rest */
			keyboard_sort_scaned_key(buf, &sc_new, tmp_sc);

			keyboard_dev.scan_buf.count--;
			i++;
		}

		//Handle toggles and caps
		if (keyboard_dev.toggles.changed) {
			//TODO debounce
			if (keyboard_dev.toggles.caps) { /* If caps is set */
				LEDs_set_caps();
				buf->mod_buf |= HID_KEYBOARD_LED_CAPSLOCK;
			} else
				LEDs_clear_caps();

			if (keyboard_dev.toggles.CLI)
				LEDs_set_CLI();
			else
				LEDs_clear_CLI();

			if (keyboard_dev.toggles.func)
				LEDs_set_func();
			else
				LEDs_clear_func();

			keyboard_dev.toggles.changed = 0;
		}

		while (buf->key_buf.count < 6 && sc_new.count) /* Fill buf with new keys */
		{
			buf->key_buf.keys[buf->key_buf.count] =
					sc_new.buf[sc_new.count - 1];
			buf->key_buf.count++;
			sc_new.count--;
		}

		/* if prev packet had contents and current doesn't then send empty packet */
		if (keyboard_dev.prev_buf.count && !buf->key_buf.count) {
			buf->key_buf.count = 1; /* trigger sending of empty packet */
			memset(&keyboard_dev.prev_buf, 0, sizeof(six_key_buffer_t));
		} else
			memcpy(&keyboard_dev.prev_buf, &buf->key_buf, /* Save for next frame */
			sizeof(six_key_buffer_t));

		if (!queue_packet_to_send) /* check queue has been created */
			goto queue_no_init;

		if (buf->key_buf.count || buf->med_buf)
			xQueueSendToFront(queue_packet_to_send, buf, portMAX_DELAY);

		xSemaphoreGive(keyboard_dev.buf_lock);
		return 0;
	}
	return -EAVAIL;
	queue_no_init:
	xSemaphoreGive(keyboard_dev.buf_lock);
	return -ENOINIT;
}
