/*
 * LED_effects.h
 *
 *  Created on: 28 Jan 2019
 *      Author: alxhoff
 */

#ifndef LED_EFFECTS_H_
#define LED_EFFECTS_H_

#include "LED_effects_config.h"
#include "config.h"

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

uint32_t Wheel_snappy_fade_out(uint8_t WheelPos) {
  WheelPos = WheelPos % 256;
  if(WheelPos < 85) {
    return newColor(WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos += 85;
    return newColor(0, WheelPos * 3, WheelPos * 3);
  }
  WheelPos += 170;
  return newColor(WheelPos * 3, WheelPos * 3, 0);
}

uint32_t Wheel_snappy_fade_in(uint8_t WheelPos) {
	  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return newColor(WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos += 85;
    return newColor(0, WheelPos * 3, WheelPos * 3);
  }
  WheelPos += 170;
  return newColor(WheelPos * 3, WheelPos * 3, 0);
}

void LED_rainbow_right(void) {
	uint32_t i;
	static uint8_t x = 0;
	static uint32_t timestamp;

	x -= 1;

	if (x == 0)
		//TODO this makes no senseg
		x = (uint8_t)(256 * 5);
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

#endif /* LED_EFFECTS_H_ */
