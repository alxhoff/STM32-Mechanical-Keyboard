/*
 * LEDs.c
 *
 *  Created on: 27 Jan 2019
 *      Author: alxhoff
 */

#include "config.h"
#include "ws2812b.h"
#include "LED_effects.h"

void LEDs_clear_caps(void) {
	HAL_GPIO_WritePin(CAPS_STATUS_PORT, CAPS_STATUS_PIN, GPIO_PIN_RESET);
}

void LEDs_set_caps(void) {
	HAL_GPIO_WritePin(CAPS_STATUS_PORT, CAPS_STATUS_PIN, GPIO_PIN_SET);
}

void LEDs_clear_CLI(void) {
	HAL_GPIO_WritePin(CLI_STATUS_PORT, CLI_STATUS_PIN, GPIO_PIN_RESET);
}

void LEDs_set_CLI(void) {
	HAL_GPIO_WritePin(CLI_STATUS_PORT, CLI_STATUS_PIN, GPIO_PIN_SET);
}

void LEDs_clear_func(void) {
	HAL_GPIO_WritePin(FUNC_STATUS_PORT, FUNC_STATUS_PIN, GPIO_PIN_RESET);
}

void LEDs_set_func(void) {
	HAL_GPIO_WritePin(FUNC_STATUS_PORT, FUNC_STATUS_PIN, GPIO_PIN_SET);
}

void LEDs_init(void) {
	LED_dev.update = LED_rainbow_right;

	for (unsigned char i = 0; i < WS2812_BUFFER_COUNT; i++) {
		ws2812b.item[i].channel = i;
		ws2812b.item[i].frameBufferPointer = LED_dev.buffers[i];
		ws2812b.item[i].frameBufferSize = sizeof(LED_dev.buffers[i]);
	}

	ws2812b_init();
}

void LEDs_run(void) {
	if (LED_dev.update)
		LED_dev.update();
	ws2812b.startTransfer = 1;
	ws2812b_handle();
}
