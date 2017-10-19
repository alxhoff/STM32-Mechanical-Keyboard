/*

  WS2812B CPU and memory efficient library

  Date: 28.9.2016

  Author: Martin Hubacek
  	  	  http://www.martinhubacek.cz
  	  	  @hubmartin

  Licence: MIT License

*/

#include <stdint.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"
#include "datatypes.h"
#include "ws2812b.h"
#include "LEDs.h"

// Helper defines
#define newColor(r, g, b) (((uint32_t)(r) << 16) | ((uint32_t)(g) <<  8) | (b))
#define Red(c) ((uint8_t)((c >> 16) & 0xFF))
#define Green(c) ((uint8_t)((c >> 8) & 0xFF))
#define Blue(c) ((uint8_t)(c & 0xFF))

uint32_t Wheel(uint8_t WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return newColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return newColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return newColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void LED_rainbow(key_devices_t* keyboard_devs)
{
	uint32_t i;
	static uint8_t x = 0;
	static uint32_t timestamp;

	x += 1;

	if(x == 256*5)
		x = 0;
	if(ws2812b.transferComplete)
	{
		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
		{
			timestamp = HAL_GetTick();
			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
				{
					uint32_t color = Wheel(((i * 256) / keyboard_devs->LEDs->rainbow_delay + x) & 0xFF);

					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
				}
			}
		}
	}
}

void visDots(uint8_t *frameBuffer, uint32_t frameBufferSize, uint32_t random, uint32_t fadeOutFactor)
{
	uint32_t i;

	for( i = 0; i < frameBufferSize / 3; i++)
	{
		if(rand() % random == 0)
		{
			frameBuffer[i*3 + 0] = 255;
			frameBuffer[i*3 + 1] = 255;
			frameBuffer[i*3 + 2] = 255;
		}

		if(frameBuffer[i*3 + 0] > fadeOutFactor)
			frameBuffer[i*3 + 0] -= frameBuffer[i*3 + 0]/fadeOutFactor;
		else
			frameBuffer[i*3 + 0] = 0;

		if(frameBuffer[i*3 + 1] > fadeOutFactor)
			frameBuffer[i*3 + 1] -= frameBuffer[i*3 + 1]/fadeOutFactor;
		else
			frameBuffer[i*3 + 1] = 0;

		if(frameBuffer[i*3 + 2] > fadeOutFactor)
			frameBuffer[i*3 + 2] -= frameBuffer[i*3 + 2]/fadeOutFactor;
		else
			frameBuffer[i*3 + 2] = 0;
	}
}

void visInit(key_devices_t* keyboard_devs)
{
	uint8_t i;

	keyboard_devs->LEDs = (LED_array_t*) calloc(1, sizeof(LED_array_t));

	//set effect parameters
	keyboard_devs->LEDs->rainbow_delay = 30;
	keyboard_devs->LEDs->rainbow_effect_length = 15;

	//set effect
	keyboard_devs->LEDs->update = &LED_rainbow;

	for( i = 0; i < WS2812_BUFFER_COUNT; i++)
	{
		ws2812b.item[i].channel = i;

		ws2812b.item[i].frameBufferPointer = keyboard_devs->LEDs->buffers[i];
		ws2812b.item[i].frameBufferSize = sizeof(keyboard_devs->LEDs->buffers[i]);
	}

	ws2812b_init();
}

void visHandle(key_devices_t* keyboard_devs)
{
	keyboard_devs->LEDs->update(keyboard_devs);
	ws2812b.startTransfer = 1;
	ws2812b_handle();
}
