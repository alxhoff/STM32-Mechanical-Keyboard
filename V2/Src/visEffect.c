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
#include <string.h>

#include "stm32f4xx_hal.h"
#include "datatypes.h"
#include "LEDs.h"

// Helper defines

//void LED_snappy_rainbow_left_fade_out(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x += 1;
//
//	if(x == 0)
//		x = 256*5;
//	if(ws2812b.transferComplete)
//	{
//		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//		{
//			timestamp = HAL_GetTick();
//			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					uint32_t color = Wheel_snappy_fade_out(((i * 256) / keyboard_devs->LEDs->rainbow_delay - x) & 0xFF);
//
//					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//				}
//			}
//		}
//	}
//}
//
//void LED_snappy_rainbow_left_fade_in(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x += 1;
//
//	if(x == 0)
//		x = 256*5;
//	if(ws2812b.transferComplete)
//	{
//		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//		{
//			timestamp = HAL_GetTick();
//			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					uint32_t color = Wheel_snappy_fade_in(((i * 256) / keyboard_devs->LEDs->rainbow_delay - x) & 0xFF);
//
//					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//				}
//			}
//		}
//	}
//}
//
//void LED_snappy_rainbow_right_fade_in(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x += 1;
//
//	if(x == 256*5)
//		x = 0;
//	if(ws2812b.transferComplete)
//	{
//		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//		{
//			timestamp = HAL_GetTick();
//			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					uint32_t color = Wheel_snappy_fade_out(((i * 256) / keyboard_devs->LEDs->rainbow_delay + x) & 0xFF);
//
//					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//				}
//			}
//		}
//	}
//}
//
//void LED_snappy_rainbow_right_fade_out(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x += 1;
//
//	if(x == 256*5)
//		x = 0;
//	if(ws2812b.transferComplete)
//	{
//		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//		{
//			timestamp = HAL_GetTick();
//			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					uint32_t color = Wheel_snappy_fade_in(((i * 256) / keyboard_devs->LEDs->rainbow_delay + x) & 0xFF);
//
//					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//				}
//			}
//		}
//	}
//}
//
//void LED_rainbow_right(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x -= 1;
//
//	if(x == 0)
//		x = 256*5;
//	if(ws2812b.transferComplete)
//	{
//		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//		{
//			timestamp = HAL_GetTick();
//			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					uint32_t color = Wheel(((i * 256) / keyboard_devs->LEDs->rainbow_delay + x) & 0xFF);
//
//					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//				}
//			}
//		}
//	}
//}
//
//void LED_rainbow_left(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x += 1;
//
//	if(x == 256*5)
//		x = 0;
//	if(ws2812b.transferComplete)
//	{
//		if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//		{
//			timestamp = HAL_GetTick();
//			for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					uint32_t color = Wheel(((i * 256) / keyboard_devs->LEDs->rainbow_delay + x) & 0xFF);
//
//					keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//				}
//			}
//		}
//	}
//}
//
//void LED_rainbow_down(key_devices_t* keyboard_devs)
//{
//	uint32_t i;
//	static uint8_t x = 0;
//	static uint32_t timestamp;
//
//	x += 1;
//
//	if(x == 256*5)
//		x = 0;
//	if((HAL_GetTick() - timestamp > keyboard_devs->LEDs->matrix_delay) &&
//				(ws2812b.startTransfer == 0) && (ws2812b.transferComplete = 1)){
//		if( xSemaphoreTake(ws2812b.transferSet, (TickType_t) 10) == pdTRUE){
//			if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->rainbow_delay)
//			{
//				timestamp = HAL_GetTick();
//				uint32_t color = Wheel((256 / keyboard_devs->LEDs->rainbow_delay + x) );
//				memcpy(keyboard_devs->LEDs->buffers[3], keyboard_devs->LEDs->buffers[2], sizeof(keyboard_devs->LEDs->buffers[0]));
//				memcpy(keyboard_devs->LEDs->buffers[2], keyboard_devs->LEDs->buffers[1], sizeof(keyboard_devs->LEDs->buffers[0]));
//				memcpy(keyboard_devs->LEDs->buffers[1], keyboard_devs->LEDs->buffers[0], sizeof(keyboard_devs->LEDs->buffers[0]));
//				for( i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++)
//				{
//					keyboard_devs->LEDs->buffers[0][i*3 + 0] = color & 0xFF;
//					keyboard_devs->LEDs->buffers[0][i*3 + 1] = color >> 8 & 0xFF;
//					keyboard_devs->LEDs->buffers[0][i*3 + 2] = color >> 16 & 0xFF;
//				}
////				for(uint8_t j = KEYBOARD_ROWS - 1; j > 0; j--){
////					memcpy(keyboard_devs->LEDs->buffers[j], keyboard_devs->LEDs->buffers[j-1], sizeof(keyboard_devs->LEDs->buffers[0]));
//
////				}
//			}
//		}
//	}
//}
//
//
//void LED_dots(key_devices_t* keyboard_devs)
//{
//	for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//		for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//			if(rand() % keyboard_devs->LEDs->dots_random == 0){
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] = 255;
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] = 255;
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] = 255;
//			}
//
//			if(keyboard_devs->LEDs->buffers[j][i*3 + 0] > keyboard_devs->LEDs->dots_fade_out)
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] -=
//						keyboard_devs->LEDs->buffers[j][i*3 + 0]/keyboard_devs->LEDs->dots_fade_out;
//			else
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] = 0;
//
//			if(keyboard_devs->LEDs->buffers[j][i*3 + 1] > keyboard_devs->LEDs->dots_fade_out)
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] -=
//						keyboard_devs->LEDs->buffers[j][i*3 + 1]/keyboard_devs->LEDs->dots_fade_out;
//			else
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] = 0;
//
//			if(keyboard_devs->LEDs->buffers[j][i*3 + 2] > keyboard_devs->LEDs->dots_fade_out)
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] -=
//						keyboard_devs->LEDs->buffers[j][i*3 + 2]/keyboard_devs->LEDs->dots_fade_out;
//			else
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] = 0;
//		}
//	}
//}
//
//void LED_RGB_dots_fade(key_devices_t* keyboard_devs)
//{
//	for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//		for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//			if(rand() % keyboard_devs->LEDs->dots_random == 0){
//				uint32_t color = Wheel((rand() % 255));
//
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//			}
//
//			if(keyboard_devs->LEDs->buffers[j][i*3 + 0] > keyboard_devs->LEDs->dots_fade_out)
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] -=
//						keyboard_devs->LEDs->buffers[j][i*3 + 0]/keyboard_devs->LEDs->dots_fade_out;
//			else
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] = 0;
//
//			if(keyboard_devs->LEDs->buffers[j][i*3 + 1] > keyboard_devs->LEDs->dots_fade_out)
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] -=
//						keyboard_devs->LEDs->buffers[j][i*3 + 1]/keyboard_devs->LEDs->dots_fade_out;
//			else
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] = 0;
//
//			if(keyboard_devs->LEDs->buffers[j][i*3 + 2] > keyboard_devs->LEDs->dots_fade_out)
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] -=
//						keyboard_devs->LEDs->buffers[j][i*3 + 2]/keyboard_devs->LEDs->dots_fade_out;
//			else
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] = 0;
//		}
//	}
//}
//
//void LED_RGB_dots_solid(key_devices_t* keyboard_devs)
//{
//	for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//		for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//			if(rand() % keyboard_devs->LEDs->dots_random == 0){
//				uint32_t color = Wheel((rand() % 255));
//
//				keyboard_devs->LEDs->buffers[j][i*3 + 0] = color & 0xFF;
//				keyboard_devs->LEDs->buffers[j][i*3 + 1] = color >> 8 & 0xFF;
//				keyboard_devs->LEDs->buffers[j][i*3 + 2] = color >> 16 & 0xFF;
//			}
//		}
//	}
//}
//
//void LED_solid(key_devices_t* keyboard_devs)
//{
//	for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//		for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//			keyboard_devs->LEDs->buffers[j][i*3 + 0] = keyboard_devs->LEDs->solid_red;
//			keyboard_devs->LEDs->buffers[j][i*3 + 1] = keyboard_devs->LEDs->solid_blue;
//			keyboard_devs->LEDs->buffers[j][i*3 + 2] = keyboard_devs->LEDs->solid_green;
//		}
//	}
//}
//
//void LED_solid_white(key_devices_t* keyboard_devs)
//{
//	for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//		for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//			keyboard_devs->LEDs->buffers[j][i*3 + 0] = 255;
//			keyboard_devs->LEDs->buffers[j][i*3 + 1] = 255;
//			keyboard_devs->LEDs->buffers[j][i*3 + 2] = 255;
//		}
//	}
//}
//
//void LED_off(key_devices_t* keyboard_devs)
//{
//	for(uint8_t j = 0; j < KEYBOARD_ROWS; j++){
//		for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//			keyboard_devs->LEDs->buffers[j][i*3 + 0] = 0;
//			keyboard_devs->LEDs->buffers[j][i*3 + 1] = 0;
//			keyboard_devs->LEDs->buffers[j][i*3 + 2] = 0;
//		}
//	}
//}
//
//void LED_null(key_devices_t* keyboard_devs){}
//
//void LED_set_row(key_devices_t* keyboard_devs)
//{
//	for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->set_row_number][i*3 + 0] = keyboard_devs->LEDs->set_row_colour & 0xFF;
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->set_row_number][i*3 + 1] = keyboard_devs->LEDs->set_row_colour >> 8 & 0xFF;
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->set_row_number][i*3 + 2] = keyboard_devs->LEDs->set_row_colour >> 16 & 0xFF;
//	}
//}
//
//void LED_count_row(key_devices_t* keyboard_devs)
//{
//	static uint32_t timestamp;
//
//	if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->count_delay){
//		timestamp = HAL_GetTick();
//
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_number]
//				 [keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] * 3 + 0] =
//						 keyboard_devs->LEDs->count_row_colour & 0xFF;
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_number]
//				 [keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] * 3 + 1] =
//						 keyboard_devs->LEDs->count_row_colour >> 8 & 0xFF;
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_number]
//				 [keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] * 3 + 2] =
//						 keyboard_devs->LEDs->count_row_colour >> 16 & 0xFF;
//
//
//		//unset prev row
//
//
//		if(keyboard_devs->LEDs->count_row_direction){
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] > 0)
//				memset(
//					&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_number]
//					  [(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] - 1) * 3]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] == KEYBOARD_COLS)
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] = 0;
//			else
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number]++;
//		}else{
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] == KEYBOARD_COLS)
//				memset(
//					&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_number][0]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//			else if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] < KEYBOARD_COLS)
//				memset(
//					&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_number]
//					  [(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] + 1) * 3]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] == 0)
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number] = KEYBOARD_COLS;
//			else
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_number]--;
//		}
//	}
//}
//
////TODO fix reverse and add col directionality
//void LED_count_all(key_devices_t* keyboard_devs)
//{
//	static uint32_t timestamp;
//
//	if(HAL_GetTick() - timestamp > keyboard_devs->LEDs->count_delay){
//		timestamp = HAL_GetTick();
//
//		//SET THE NEW POSITION
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index]
//				 [keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] * 3 + 0] =
//						 keyboard_devs->LEDs->count_row_colour & 0xFF;
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index]
//				 [keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] * 3 + 1] =
//						 keyboard_devs->LEDs->count_row_colour >> 8 & 0xFF;
//		keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index]
//				 [keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] * 3 + 2] =
//						 keyboard_devs->LEDs->count_row_colour >> 16 & 0xFF;
//
//		//IF FORWARDS
//		if(keyboard_devs->LEDs->count_row_direction){
//			//IF NOT THE FIRST LED IN A ROW
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] > 0)
//				memset(
//					&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index]
//					  [(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] - 1) * 3]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//			//ELSE FIRST LED IN A ROW
//			else{
//				//IF FIRST ROW ON KEYBOARD
//				if(keyboard_devs->LEDs->count_row_index == 0)
//					memset(
//						&keyboard_devs->LEDs->buffers[KEYBOARD_ROWS - 1][(KEYBOARD_COLS - 1) * 3]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//				else
//					memset(
//						&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index - 1]
//						  [(KEYBOARD_COLS - 1) * 3]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//			}
//
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] == KEYBOARD_COLS - 1){
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] = 0;
//
//				goto handle_row;
//			}
//			else
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index]++;
//
//		}else{
//			//IF NOT THE FIRST LED IN A ROW
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] < (KEYBOARD_COLS - 1))
//				memset(
//					&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index]
//					  [(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] + 1) * 3]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//			//ELSE FIRST LED IN ROW
//			else
//				//IF LAST ROW ON KEYBOARD
//				if(keyboard_devs->LEDs->count_row_index == (KEYBOARD_ROWS - 1))
//					memset(
//						&keyboard_devs->LEDs->buffers[0][0]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//				else
//					memset(
//						&keyboard_devs->LEDs->buffers[keyboard_devs->LEDs->count_row_index + 1]
//						  [0]
//													   , 0x00
//													   , sizeof(uint8_t) * 3);
//			//IF LAST LED IN ROW
//			if(keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] == 0){
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index] = KEYBOARD_COLS - 1;
//
//				goto handle_row;
//			}
//			else
//				keyboard_devs->LEDs->count_col_indexs[keyboard_devs->LEDs->count_row_index]--;
//		}
//	}
//	return;
//	handle_row:
//		if(keyboard_devs->LEDs->count_col_direction){
//			if(keyboard_devs->LEDs->count_row_index == (KEYBOARD_ROWS - 1))
//				keyboard_devs->LEDs->count_row_index = 0;
//			else
//				keyboard_devs->LEDs->count_row_index++;
//		}else{
//			if(keyboard_devs->LEDs->count_row_index == 0)
//				keyboard_devs->LEDs->count_row_index = KEYBOARD_ROWS - 1;
//			else
//				keyboard_devs->LEDs->count_row_index--;
//		}
//}
//
//void LED_matrix(key_devices_t* keyboard_devs)
//{
//	static uint32_t timestamp;
//
//	if((HAL_GetTick() - timestamp > keyboard_devs->LEDs->matrix_delay)){
//		timestamp = HAL_GetTick();
//
//		if( xSemaphoreTake(ws2812b.transferSet, (TickType_t) 10) == pdTRUE){
//			for(uint8_t j = KEYBOARD_ROWS -1; j > 0; j--){
//				memcpy(keyboard_devs->LEDs->buffers[j], keyboard_devs->LEDs->buffers[j-1],
//						sizeof(keyboard_devs->LEDs->buffers[0]));
//			}
//			memset(keyboard_devs->LEDs->buffers[0], 0x00, sizeof(keyboard_devs->LEDs->buffers[0]));
//			for(uint8_t i = 0; i < sizeof(keyboard_devs->LEDs->buffers[0]) / 3; i++){
//
//				//create ghost of now second row in first row
//				if(KEYBOARD_ROWS >= 2){
//					//first row
//					keyboard_devs->LEDs->buffers[0][i*3 + 0] = ((keyboard_devs->LEDs->buffers[1][i*3 + 0]
//																>> keyboard_devs->LEDs->matrix_fade_amount));
//					keyboard_devs->LEDs->buffers[0][i*3 + 1] = ((keyboard_devs->LEDs->buffers[1][i*3 + 1]
//																>> keyboard_devs->LEDs->matrix_fade_amount));
//					keyboard_devs->LEDs->buffers[0][i*3 + 2] = ((keyboard_devs->LEDs->buffers[1][i*3 + 2]
//																>> keyboard_devs->LEDs->matrix_fade_amount));
//				}
//				//create ghost of now third row in first row
////				if(KEYBOARD_ROWS >= 3){
////					//first row
////					keyboard_devs->LEDs->buffers[0][i*3 + 0] |=  (keyboard_devs->LEDs->buffers[2][i*3 + 0] >> (2 * keyboard_devs->LEDs->matrix_fade_amount));
////					keyboard_devs->LEDs->buffers[0][i*3 + 1] |=  (keyboard_devs->LEDs->buffers[2][i*3 + 1] >> (2 * keyboard_devs->LEDs->matrix_fade_amount));
////					keyboard_devs->LEDs->buffers[0][i*3 + 2] |=  (keyboard_devs->LEDs->buffers[2][i*3 + 2] >> (2 * keyboard_devs->LEDs->matrix_fade_amount));
////				}
//
////				set first row
//				if((rand() % 100) < keyboard_devs->LEDs->matrix_probability &&
//						!(keyboard_devs->LEDs->buffers[0][i*3 + 0] != keyboard_devs->LEDs->matrix_red &&
//								keyboard_devs->LEDs->buffers[0][i*3 + 1] != keyboard_devs->LEDs->matrix_blue &&
//								keyboard_devs->LEDs->buffers[0][i*3 + 2] != keyboard_devs->LEDs->matrix_green)){
//					keyboard_devs->LEDs->buffers[0][i*3 + 0] = keyboard_devs->LEDs->matrix_red ;
//					keyboard_devs->LEDs->buffers[0][i*3 + 1] = keyboard_devs->LEDs->matrix_blue;
//					keyboard_devs->LEDs->buffers[0][i*3 + 2] = keyboard_devs->LEDs->matrix_green;
//				}
//			}
//		}
//	}
//}

//void visInit(void)
//{
//	uint8_t i;
//
//	keyboard_devs->LEDs = (LED_array_t*) calloc(1, sizeof(LED_array_t));
//
//	keyboard_devs->LEDs->inverted_rows = 1;
//
//	//set effect parameters
//	keyboard_devs->LEDs->rainbow_delay = 15;
//	keyboard_devs->LEDs->rainbow_effect_length = 15;
//
//	keyboard_devs->LEDs->dots_fade_out= 40;
//	keyboard_devs->LEDs->dots_random = 50;
//
//	keyboard_devs->LEDs->solid_red = 0;
//	keyboard_devs->LEDs->solid_green = 0;
//	keyboard_devs->LEDs->solid_blue = 255;
//
//	keyboard_devs->LEDs->matrix_delay = 1000;
//	keyboard_devs->LEDs->matrix_probability = 25;
//	keyboard_devs->LEDs->matrix_fade_amount = 2;
//	keyboard_devs->LEDs->matrix_red = 0;
//	keyboard_devs->LEDs->matrix_green = 180;
//	keyboard_devs->LEDs->matrix_blue = 0;
//
//	keyboard_devs->LEDs->set_row_number = 0;
//	keyboard_devs->LEDs->set_row_colour = 0xFFFFFF;
//
//	keyboard_devs->LEDs->count_row_number = 4;
//	keyboard_devs->LEDs->count_delay = 500;
//	keyboard_devs->LEDs->count_row_colour = 0xFFFFFF;
//	keyboard_devs->LEDs->count_row_direction = 1;
//	keyboard_devs->LEDs->count_col_direction = 1;
//
//	//set effect
//	keyboard_devs->LEDs->update = &LED_rainbow_left;
//
//	for( i = 0; i < WS2812_BUFFER_COUNT; i++)
//	{
//		ws2812b.item[i].channel = i;
//		if(keyboard_devs->LEDs->inverted_rows){
//			ws2812b.item[i].frameBufferPointer = keyboard_devs->LEDs->buffers[WS2812_BUFFER_COUNT - i - 1];
//		}else{
//			ws2812b.item[i].frameBufferPointer = keyboard_devs->LEDs->buffers[i];
//		}
//		ws2812b.item[i].frameBufferSize = sizeof(keyboard_devs->LEDs->buffers[i]);
//	}
//
//	ws2812b_init();
//}
//
//void visHandle(void)
//{
////	keyboard_devs->LEDs->update(keyboard_devs);
//	ws2812b.startTransfer = 1;
//	ws2812b_handle();
//}
