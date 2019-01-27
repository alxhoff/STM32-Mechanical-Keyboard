/*
 * LEDs.c
 *
 *  Created on: 27 Jan 2019
 *      Author: alxhoff
 */

#include "ws2812b.h"
#include "stm32f4xx_hal.h"
#include "LED_effects.h"


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
	if(LED_dev.update)
		LED_dev.update();
	ws2812b.startTransfer = 1;
	ws2812b_handle();
}
