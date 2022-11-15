/**
 * @file keyboard.c
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Data types and functions for performing classical keyboard functions
 */

#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"

#include "HIDClassCommon.h"

#include "buffers.h"
#include "config.h"
#include "sn54hc595.h"
#include "keymap.h"
#include "lookup.h"
#include "LEDs.h"

#define TOGGLE_DELAY		200

#define SCAN_BUFF_END BUFFER_GET_END(kb.scan_buff)
#define SEND_BUFF_SET_END(VALUE) BUFFER_SET_END(kb.send_buff.key_buff, VALUE)

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
	GPIO_TypeDef *row_ports[KEYBOARD_ROWS]; /**< GPIO ports used by the keyboard rows */

	const unsigned char keyboard_report_ID;
	const unsigned char media_report_ID;

	//TODO lock this to prepare for multicore
	SemaphoreHandle_t buff_lock;
	key_buffer_t scan_buff; /**< Buffer where scanned keys are stored before processing */
	send_buffer_t send_buff;
//	key_buffer_t prev_buff;

	toggle_keys_t toggles;

	TimerHandle_t caps_debounce_timer;
	SemaphoreHandle_t caps_lock;

	TaskHandle_t scan_task_handle;
} keyboard_device_t;

QueueHandle_t keyboard_send_buffer_queue;

keyboard_device_t kb = { .row_ports = { ROW_PORT_0, ROW_PORT_1,
ROW_PORT_2, ROW_PORT_3, ROW_PORT_4 }, .row_pins = { ROW_PIN_0,
ROW_PIN_1, ROW_PIN_2, ROW_PIN_3, ROW_PIN_4 }, .keyboard_report_ID = 1,
		.media_report_ID = 2, };

void keyboardCapsDebounceCallback(TimerHandle_t timer) {
	xSemaphoreGive(kb.caps_lock);
}

static unsigned char keyboardReadRow(unsigned char row) {
	return HAL_GPIO_ReadPin(kb.row_ports[row], kb.row_pins[row]);
}

static void keyboardScanBufferAdd(unsigned char key_code) {
	kb.scan_buff.buff[kb.scan_buff.count] = key_code;
	kb.scan_buff.count++;
}

signed char keyboardScanMatrix(void) {

	static unsigned short row_mask = { 0 };

	if ( xSemaphoreTake(kb.buff_lock, (TickType_t) 0) == pdFALSE)
		return 1;

	for (unsigned char col = 0; col < KEYBOARD_COLS; col++) { /* Set each col high and test rows*/
		row_mask = (1 << col);

		if (SN54HC595_out_bytes((unsigned char*) &row_mask, SHIFT_DEVICES))
			goto error;
		vTaskDelay(1);

		for (unsigned char row = 0; row < KEYBOARD_ROWS; row++) /* test each row */
			if (keyboardReadRow(row)) /*key is pressed */
				keyboardScanBufferAdd(
						keymapGetKeyCode(KEYBOARD_COLS - col - 1, row)); //TODO remove - requirement
	}

	xSemaphoreGive(kb.buff_lock);

	if (kb.scan_buff.count)
		return 0; // Buffer needs processing
	else
		return 1; // Nothing to do

	error:
	xSemaphoreGive(kb.buff_lock);
	return 1;
}

// sort all standard keys: normal, mod and media into buffer
//returns -1 if 6 normals keys have been processed
static void keyboardFillSendBuffer(unsigned char key) {
	if (key >= 0xE8 && key <= 0xEF) /* media */
	{
		kb.send_buff.med_buff = key;
		return;
	}

	if (key >= 0xE0 && key <= 0xE7) /* modifier */
	{
		kb.send_buff.mod_buff |= (1 << (key - 0xE0));
		return;
	}

	SEND_BUFF_SET_END(key);
	//sort key into send buffer or new keys buff
//	if (keyboardInPrevBuffer(key))
//		send_buf->key_buf.keys[send_buf->key_buf.count++] = key;
//	else
//		buf_new->buff[buf_new->count++] = key;
}

void keyboardCapsToggle(void) {
	kb.toggles.caps ^= 1;
}

void keyboardCLIToggle(void) {
	kb.toggles.CLI ^= 1;
}

void keyboardFuncToggle(void) {
	kb.toggles.func ^= 1;
}

void keyboardCheckCaps() {
	if (lookupCaps(SCAN_BUFF_END)) {
		if (xSemaphoreTake(kb.caps_lock, 0)) {
			xTimerReset(kb.caps_debounce_timer,
			portMAX_DELAY);
			keyboardCapsToggle();
			LEDsToggleCaps();
			kb.send_buff.mod_buff |= HID_KEYBOARD_LED_CAPSLOCK;
		}
	}
}

/**
 * @brief Processes the input key buffer
 *
 * Once the keyboard scan is complete and the input key buffer has been
 * populated the buffer must be processed. The keyboard state is first handled,
 * going to state code if needed. The buffer is then processed, sorting keys
 * into either modifier keys, media keys or keyboard keys. Once sorted the
 * key codes or bitmasks are retrieved/set. Normal keyboard button presses
 * are first compared against the last HID report as repeating keys get priority
 * in the the next HID report as they represent keys being held down. Keys not
 * in the previous HID report are shortlisted such that after all keys have been
 * converted into key codes the HID report can be filled with shortlisted keys.
 *
 * @param data global keyboard HID data
 * @param layer_list layer list used to convert key_codes
 * @return 0 on success
 */
signed char keyboardProcessScanBuff(void) {

	if (xSemaphoreTake(kb.buff_lock, (TickType_t) 0) == pdTRUE) {
		// Clear send buffer
		memset(&kb.send_buff, 0, sizeof(send_buffer_t));

		for (; kb.scan_buff.count; kb.scan_buff.count--) {

			// State change
			if (lookupCheckStateChange(SCAN_BUFF_END)) { /* if state change key */
				kb.scan_buff.count = 0; /* clear buffer */
				return 0;
			}

			// Caps
			keyboardCheckCaps();

			// Process other key presses into send buffer
			keyboardFillSendBuffer(SCAN_BUFF_END);
		}

		if (!keyboard_send_buffer_queue) /* check queue has been created */
			goto queue_no_init;

		if (kb.send_buff.key_buff.count || kb.send_buff.med_buff)
			xQueueSendToFront(keyboard_send_buffer_queue, &kb.send_buff,
					portMAX_DELAY);

		xSemaphoreGive(kb.buff_lock);
		return 0;
	}
	return 1;
	queue_no_init:
	xSemaphoreGive(kb.buff_lock);
	return 1;
}

void keyboardScanTask(void *args) {
	TickType_t xLastWakeTime = xTaskGetTickCount();
	TickType_t xPeriod = 10;

	/* Infinite loop */
	for (;;) {
		//HID TEST
		if (keyboardScanMatrix() == 0) /* if keys were pressed */
			keyboardProcessScanBuff(); /* convert row-col to actual keys */

		xPeriod = SCAN_PERIOD - (xLastWakeTime - xTaskGetTickCount());
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
}

static inline void keyboardInitLEDGPIOs(void) {
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

static inline void keyboardInitRowGPIOs(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	//INIT ROWS - input
	for (unsigned char i = 0; i < KEYBOARD_ROWS; i++) {
		GPIO_InitStruct.Pin = kb.row_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(kb.row_ports[i], &GPIO_InitStruct);
	}
}

static inline void keyboardInitBuffers(void) {
	keyboard_send_buffer_queue = xQueueCreate(20, sizeof(send_buffer_t));
	kb.buff_lock = xSemaphoreCreateMutex();
}

static inline void keyboardInitCaps(void) {
	kb.caps_debounce_timer = xTimerCreate("Cursor Timer",
	TOGGLE_DELAY, 0, NULL, keyboardCapsDebounceCallback);

	kb.caps_lock = xSemaphoreCreateBinary();
	xSemaphoreGive(kb.caps_lock);
}

signed char keyboardInit(void) {

	// LED Output GPIOs
	keyboardInitLEDGPIOs();
	// Input row GPIOs
	keyboardInitRowGPIOs();

	keyboardInitBuffers();
	// Input timer and semaphore for caps lock
	keyboardInitCaps();

	// Scan task
	xTaskCreate(keyboardScanTask, "keyboard_scan_task", 128, NULL, 1,
			&kb.scan_task_handle);

	return 0;
}
