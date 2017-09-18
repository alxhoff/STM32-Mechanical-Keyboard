/*
 * SN54HC595.h
 *
 *  Created on: Sep 17, 2017
 *      Author: alxhoff
 */

#ifndef SN54HC595_H_
#define SN54HC595_H_

/*
 *#####PINOUT#####
 *
 *	OUTPUT 0 - Pin 15
 *	OUTPUT 1 - Pin 1
 *	OUTPUT 2 - Pin 2
 *	OUTPUT 3 - Pin 3
 *	OUTPUT 4 - Pin 4
 *	OUTPUT 5 - Pin 5
 *	OUTPUT 6 - Pin 6
 *	OUTPUT 7 - Pin 7s
 *
 *	Serial data in (MOSI equiv) - Pin 14
 *	Serial data out				- Pin 9
 *	(data out to next chip)
 *	Serial clock				- Pin 11
 *
 *	Latch register				- Pin 12
 *
 *	Latch pin clocks internal register
 *	data to output pins
 *
 *	Output enable (active low)	- Pin 13
 *
 *	Shift register clear		- Pin 10
 *	(active low)
 *
 *	How chip works:
 *	Data is clocked in using serial data in and
 *	serial clock (on falling edge) then the internal
 *	storage register is clocked to the output pins
 *	once the latch pin is clocked high then low
 *	(rising edge).
 *
 *	The internal storage register overflows out of
 *	the serial out pin to allow for daisy chaining
 *	of chips.
 *
 *	#####HOW MY CODE WORKS#####
 *	I have implemented my code in two styles,
 *	one uses a simple number of functions that can
 *	be used to use a status register attached to
 *	the pins specified in the #defines or it can
 *	be used with the shift_array_t object. The
 *	object code allows for multiple shift arrays to
 *	be easily connect to and managed from a single
 *	uController.
 *
 *	To use the struct obj, set the pins and ports of
 *	the object then call SN54HC595_init_obj as seen
 *	in the example.
 */

//PIN DEFS

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

#define SER_IN_PIN			GPIO_PIN_8 //A8
#define SER_IN_PORT			GPIOA
#define SER_IN_CLOCK			CLOCK_ENABLE(GPIOA)
#define SER_CLK_PIN			GPIO_PIN_7 //G7
#define SER_CLK_PORT			GPIOG
#define SER_CLK_CLOCK			CLOCK_ENABLE(GPIOG)

#define LATCH_PIN			GPIO_PIN_6 //C6
#define LATCH_PORT			GPIOC
#define LATCH_CLOCK			CLOCK_ENABLE(GPIOC)

#define OUT_ENA_PIN			GPIO_PIN_8 //C8
#define OUT_ENA_PORT			GPIOC
#define OUT_ENA_CLOCK			CLOCK_ENABLE(GPIOC)
#define SR_CLR_PIN			GPIO_PIN_5 //G5
#define SR_CLR_PORT			GPIOG
#define SR_CLR_CLOCK			CLOCK_ENABLE(GPIOG)

#define USE_SN54HC595_STRUCTS		1

#ifdef USE_SN54HC595_STRUCTS
typedef struct shift_array shift_array_t;

struct shift_array{
	uint8_t* out_buf;

	uint8_t dev_count;

	uint16_t ser_in_pin;
	GPIO_TypeDef* ser_in_port;
	uint8_t ser_in_clock_init;
	uint16_t ser_clk_pin;
	GPIO_TypeDef* ser_clk_port;
	uint8_t ser_clk_clock_init;

	uint16_t latch_pin;
	GPIO_TypeDef* latch_port;
	uint8_t latch_clock_init;

	uint16_t out_ena_pin;
	GPIO_TypeDef* out_ena_port;
	uint8_t out_ena_clock_init;
	uint8_t out_ena_connected;

	uint16_t sr_clr_pin;
	GPIO_TypeDef* sr_clr_port;
	uint8_t sr_clr_clock_init;
	uint8_t sr_clr_connected;

	void (* output)(shift_array_t*);

	void (* output_delay)(shift_array_t*, uint32_t);

	void (* disbale)(shift_array_t*);

	void (* enable)(shift_array_t*);

	void (* reset_latch)(shift_array_t*);

	void (* set_byte)(shift_array_t*, uint8_t, uint8_t);

	void (* set_data)(shift_array_t*, uint8_t*);
};

#endif

void SN54HC595_init();
void SN54HC595_latch_register();
void SN54HC595_clock_register();
void SN54HC595_clear_register();
void SN54HC595_disable();
void SN54HC595_enable();
void SN54HC595_reset_latch();
void SN54HC595_out_bytes(uint8_t* data, uint8_t byte_count);
void SN54HC595_out_bytes_w_delay(uint8_t* data, uint8_t byte_count, uint32_t delay);

#ifdef USE_SN54HC595_STRUCTS

void output_self(shift_array_t* self);
void output_self_delay(shift_array_t* self, uint32_t delay);
void disable_self(shift_array_t* self);
void enable_self(shift_array_t* self);
void reset_latch_self(shift_array_t* self);
void set_byte_self(shift_array_t* self, uint8_t byte_index,uint8_t byte);
void set_data_self(shift_array_t* self, uint8_t* data);
void SN54HC595_init_obj(shift_array_t* self);
#endif

#endif /* SN54HC595_H_ */
