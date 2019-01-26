/*
 * SN54HC595.c
 *
 *  Created on: Sep 17, 2017
 *      Author: alxhoff
 */

#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "SN54HC595.h"
#include "error.h"

#define CLOCK_SWITCH(PORT)		{if(PORT == GPIOA)\
							__HAL_RCC_GPIOA_CLK_ENABLE();\
						else if(PORT == GPIOB) \
							__HAL_RCC_GPIOB_CLK_ENABLE();\
						else if(PORT == GPIOC) \
							__HAL_RCC_GPIOC_CLK_ENABLE();\
						else if(PORT == GPIOD) \
							__HAL_RCC_GPIOD_CLK_ENABLE();\
						else if(PORT == GPIOE) \
							__HAL_RCC_GPIOE_CLK_ENABLE();\
						else if(PORT == GPIOF) \
							__HAL_RCC_GPIOF_CLK_ENABLE();\
						else if(PORT == GPIOG) \
							__HAL_RCC_GPIOG_CLK_ENABLE();\
						else if(PORT == GPIOH) \
							__HAL_RCC_GPIOH_CLK_ENABLE();\
						else if(PORT == GPIOI) \
							__HAL_RCC_GPIOI_CLK_ENABLE();\
						else if(PORT == GPIOJ) \
							__HAL_RCC_GPIOJ_CLK_ENABLE();\
						else if(PORT == GPIOK) \
							__HAL_RCC_GPIOK_CLK_ENABLE();\
						}

#define CLOCK_ENABLE(PORT)		__HAL_RCC_##PORT##_CLK_ENABLE();

typedef struct shift_array shift_array_t;

struct shift_array{
	unsigned char* out_buf;

	unsigned char dev_count;

	uint16_t ser_in_pin;
	GPIO_TypeDef* ser_in_port;
	unsigned char ser_in_clock_init;
	uint16_t ser_clk_pin;
	GPIO_TypeDef* ser_clk_port;
	unsigned char ser_clk_clock_init;

	uint16_t latch_pin;
	GPIO_TypeDef* latch_port;
	unsigned char latch_clock_init;

	uint16_t out_ena_pin;
	GPIO_TypeDef* out_ena_port;
	unsigned char out_ena_clock_init;
	unsigned char out_ena_connected;

	uint16_t sr_clr_pin;
	GPIO_TypeDef* sr_clr_port;
	unsigned char sr_clr_clock_init;
	unsigned char sr_clr_connected;

	void (* output)(shift_array_t*, unsigned char);

	void (* output_delay)(shift_array_t*, unsigned char, uint32_t);

	void (* disbale)(shift_array_t*);

	void (* enable)(shift_array_t*);

	void (* reset_latch)(shift_array_t*);

	void (* set_byte)(shift_array_t*, unsigned char, unsigned char);

	void (* set_data)(shift_array_t*, unsigned char*);

	void (* clock_data)(shift_array_t*);

	void (* latch)(shift_array_t*);
};

shift_array_t shift_dev = {
				.dev_count = SHIFT_DEVICES,
				.ser_in_pin = SHIFT_SER_IN_PIN,	//GPIO PINS FOR SHIFT ARRAY
				.ser_in_port = SHIFT_SER_IN_PORT,
				.ser_in_clock_init = 1,
				.ser_clk_pin = SHIFT_SER_CLK_PIN,
				.ser_clk_port = SHIFT_SER_CLK_PORT,
				.ser_clk_clock_init = 1,
				.latch_pin = SHIFT_LATCH_PIN,
				.latch_port = SHIFT_LATCH_PORT,
				.latch_clock_init = 1,
				.out_ena_pin = SHIFT_OUT_ENA_PIN,
				.out_ena_port = SHIFT_OUT_ENA_PORT,
				.out_ena_clock_init = 1,
				.sr_clr_pin = SHIFT_SER_CLR_PIN,
				.sr_clr_port = SHIFT_SER_CLR_PORT,
				.sr_clr_clock_init = 1,
		};

unsigned char SN54HC595_get_dev_count(void)
{
	return shift_dev.dev_count;
}

unsigned char SN54HC595_out_bytes(unsigned char* data, unsigned char byte_count)
{
	if(byte_count != shift_dev.dev_count)
			return -EINVAL;
	shift_dev.set_data(&shift_dev, data);
	shift_dev.output(&shift_dev, byte_count);

	return 0;
}

//TODO is byte count necessary?
unsigned char SN54HC595_out_bytes_delay(unsigned char* data,
		unsigned char byte_count, unsigned long delay)
{
	if(byte_count != shift_dev.dev_count)
		return -EINVAL;
	shift_dev.set_data(&shift_dev, data);
	shift_dev.output_delay(&shift_dev, byte_count, delay);

	return 0;
}

void SN54HC595_out(void)
{
	shift_dev.output(&shift_dev, shift_dev.dev_count);
}

void SN54HC595_clear(void)
{
	HAL_GPIO_WritePin(shift_dev.ser_clk_port, shift_dev.ser_clk_pin, GPIO_PIN_RESET);
	for(unsigned char i = 0; i < shift_dev.dev_count * 8; i++){
		HAL_GPIO_WritePin(shift_dev.ser_in_port, shift_dev.ser_in_pin, GPIO_PIN_RESET);
		shift_dev.clock_data(&shift_dev);
	}
	//latch data
	shift_dev.latch(&shift_dev);
}

void output_self(shift_array_t* self, unsigned char byte_count)
{
	//Set serial clock and latch pin low
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_RESET);
	for(unsigned char i = byte_count; i > 0; i--){
		for(unsigned char j = 8; j > 0; j--){
			if(*(self->out_buf + ((i-1) * sizeof(unsigned char))) & (1 << (j-1)))
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_RESET);
			//clock bit
			self->clock_data(self);
		}
	}
	//latch data
	self->latch(self);
}

void output_self_delay(shift_array_t* self, unsigned char byte_count, uint32_t delay)
{
	//Set serial clock and latch pin low
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_RESET);
	for(unsigned char i = 0; i < byte_count; i++){
		for(unsigned char j = 0; j < 8; j++){
			if(*(self->out_buf + (i * sizeof(unsigned char))) & (1 << j))
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(self->ser_in_port, self->ser_in_pin, GPIO_PIN_RESET);
			//clock bit
			self->clock_data(self);
		}
	//latch data
	self->latch(self);
	HAL_Delay(delay);
	}
}

void disable_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->out_ena_port, self->out_ena_pin, GPIO_PIN_SET);
}

void enable_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->out_ena_port, self->out_ena_pin, GPIO_PIN_RESET);
}

void reset_latch_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->latch_port, self->latch_pin, GPIO_PIN_RESET);
}

void set_byte_self(shift_array_t* self, unsigned char byte_index, unsigned char byte)
{
	if(byte_index < self->dev_count)
		self->out_buf[byte_index] = byte;
}

void set_data_self(shift_array_t* self, unsigned char* data)
{
	memcpy(self->out_buf, data, self->dev_count * sizeof(unsigned char));
}

void clock_data_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(self->ser_clk_port, self->ser_clk_pin, GPIO_PIN_RESET);
}

void latch_self(shift_array_t* self)
{
	HAL_GPIO_WritePin(self->latch_port, self->latch_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(self->latch_port, self->latch_pin, GPIO_PIN_RESET);
}

void SN54HC595_init_obj(shift_array_t* shift)
{
//	//CLOCK ENABLE STUFF
//	if(self->ser_in_clock_init)
//		CLOCK_SWITCH(self->ser_in_port);
//	if((self->ser_clk_port != self->ser_in_port) && self->ser_clk_clock_init)
//		CLOCK_SWITCH(self->ser_clk_port);
//	if((self->latch_port != self->ser_in_port && self->latch_port != self->ser_clk_port)
//			&& self->latch_clock_init)
//		CLOCK_SWITCH(self->latch_port);
//	if(self->out_ena_connected && self->out_ena_clock_init)
//		CLOCK_SWITCH(self->out_ena_port);
//	if(self->sr_clr_connected && self->sr_clr_clock_init)
//		CLOCK_SWITCH(self->sr_clr_port);

	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure serial in */
	GPIO_InitStruct.Pin = shift->ser_in_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(shift->ser_in_port, &GPIO_InitStruct);

	/*Configure serial clock */
	GPIO_InitStruct.Pin = shift->ser_clk_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(shift->ser_clk_port, &GPIO_InitStruct);

	/*Configure latch pin */
	GPIO_InitStruct.Pin = shift->latch_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(shift->latch_port, &GPIO_InitStruct);

	/*Configure output enable */
	GPIO_InitStruct.Pin = shift->out_ena_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(shift->out_ena_port, &GPIO_InitStruct);

	/*Configure serial clear */
	GPIO_InitStruct.Pin = shift->sr_clr_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(shift->sr_clr_port, &GPIO_InitStruct);

	enable_self(shift);

	reset_latch_self(shift);

	shift->out_buf = (unsigned char*)calloc(1, sizeof(unsigned char)* shift->dev_count);

	for(int i = 0; i < shift->dev_count; i++)
		shift->out_buf[i] = 0xFF;

	HAL_GPIO_WritePin(shift->sr_clr_port, shift->sr_clr_pin, GPIO_PIN_SET);

	shift->output = &output_self;
	shift->output_delay = &output_self_delay;
	shift->disbale = &disable_self;
	shift->enable = &enable_self;
	shift->reset_latch = &reset_latch_self;
	shift->set_byte = &set_byte_self;
	shift->set_data = &set_data_self;
	shift->clock_data = &clock_data_self;
	shift->latch = &latch_self;
}

void SN54HC595_init()
{
	SN54HC595_init_obj(&shift_dev);
}
