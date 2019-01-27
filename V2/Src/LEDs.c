/*
 * LEDs.c
 *
 *  Created on: 27 Jan 2019
 *      Author: alxhoff
 */

#include "ws2812b.h"
#include "LED_effects_config.h"
#include "stm32f4xx_hal.h"

#define CHECK_EVENODD(N) ((N%2==0)?1:0)

#if CHECK_EVENODD(KEYBOARD_COLS)
#define LED_ROW_BUFF_LENGTH 	KEYBOARD_COLS
#else
#define LED_ROW_BUFF_LENGTH		(KEYBOARD_COLS + 1)
#endif

#define newColor(r, g, b) (((uint32_t)(r) << 16) | ((uint32_t)(g) <<  8) | (b))
#define Red(c) ((uint8_t)((c >> 16) & 0xFF))
#define Green(c) ((uint8_t)((c >> 8) & 0xFF))
#define Blue(c) ((uint8_t)(c & 0xFF))

typedef struct LED_array {
	uint8_t buffers[KEYBOARD_ROWS][LED_ROW_BUFF_LENGTH*3];

	void (*update)(void);
}LED_array_t;

LED_array_t LED_dev = { 0 };

uint32_t Wheel(uint8_t WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return newColor(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return newColor(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return newColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void LED_rainbow_right(void) {
	uint32_t i;
	static uint8_t x = 0;
	static uint32_t timestamp;

	x -= 1;

	if (x == 0)
		x = 256 * 5;
	if (ws2812b.transferComplete) {
		if (HAL_GetTick() - timestamp > RAINBOW_RIGHT_DELAY) {
			timestamp = HAL_GetTick();
			for (uint8_t j = 0; j < KEYBOARD_ROWS; j++) {
				for (i = 0; i < sizeof(LED_dev.buffers[0]) / 3; i++) {
					uint32_t color = Wheel(
							((i * 256) / RAINBOW_RIGHT_DELAY + x) & 0xFF);

					LED_dev.buffers[j][i * 3 + 0] = color & 0xFF;
					LED_dev.buffers[j][i * 3 + 1] = color >> 8 & 0xFF;
					LED_dev.buffers[j][i * 3 + 2] = color >> 16 & 0xFF;
				}
			}
		}
	}
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
	if(LED_dev.update)
		LED_dev.update();
	ws2812b.startTransfer = 1;
	ws2812b_handle();
}
