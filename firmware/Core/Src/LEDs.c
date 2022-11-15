/*
 * LEDs.c
 *
 *  Created on: 27 Jan 2019
 *      Author: alxhoff
 */

#include "config.h"
#include "ws2812b.h"
#include "LED_effects.h"

void LEDsToggleCaps(void) {
	HAL_GPIO_TogglePin(CAPS_STATUS_PORT, CAPS_STATUS_PIN);
}

void LEDsClearCaps(void) {
	HAL_GPIO_WritePin(CAPS_STATUS_PORT, CAPS_STATUS_PIN, GPIO_PIN_RESET);
}

void LEDsSetCaps(void) {
	HAL_GPIO_WritePin(CAPS_STATUS_PORT, CAPS_STATUS_PIN, GPIO_PIN_SET);
}

void LEDsClearCLI(void) {
	HAL_GPIO_WritePin(CLI_STATUS_PORT, CLI_STATUS_PIN, GPIO_PIN_RESET);
}

void LEDsSetCLI(void) {
	HAL_GPIO_WritePin(CLI_STATUS_PORT, CLI_STATUS_PIN, GPIO_PIN_SET);
}

void LEDsClearFunc(void) {
	HAL_GPIO_WritePin(FUNC_STATUS_PORT, FUNC_STATUS_PIN, GPIO_PIN_RESET);
}

void LEDsSetFunc(void) {
	HAL_GPIO_WritePin(FUNC_STATUS_PORT, FUNC_STATUS_PIN, GPIO_PIN_SET);
}

void LEDsInit(void) {
	LED_dev.update = LED_rainbow_right;

	for (unsigned char i = 0; i < WS2812_BUFFER_COUNT; i++) {
		ws2812b.item[i].channel = i;
		ws2812b.item[i].frameBufferPointer = LED_dev.buffers[i];
		ws2812b.item[i].frameBufferSize = sizeof(LED_dev.buffers[i]);
	}

	ws2812bInit();
}

void LEDsRun(void) {
	if (LED_dev.update)
		LED_dev.update();
	ws2812b.startTransfer = 1;
	ws2812bHandle();
}
