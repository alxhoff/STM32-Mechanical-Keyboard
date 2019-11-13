/*
 * SN54HC595.h
 *
 *  Created on: Sep 17, 2017
 *      Author: alxhoff
 */

#ifndef SN54HC595_H_
#define SN54HC595_H_

#include "config.h"

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
#define SER_IN_PIN			SHIFT_SER_IN_PIN //A8
#define SER_IN_PORT			SHIFT_SER_IN_PORT
#define SER_IN_CLOCK		CLOCK_ENABLE(SHIFT_SER_IN_PORT)
#define SER_CLK_PIN			SHIFT_SER_CLK_PIN //G7
#define SER_CLK_PORT		SHIFT_SER_CLK_PORT
#define SER_CLK_CLOCK		CLOCK_ENABLE(SHIFT_SER_CLK_PORT)

#define LATCH_PIN			SHIFT_LATCH_PIN //C6
#define LATCH_PORT			SHIFT_LATCH_PORT
#define LATCH_CLOCK			CLOCK_ENABLE(SHIFT_LATCH_PORT)

#define OUT_ENA_PIN			SHIFT_OUT_ENA_PIN //C8
#define OUT_ENA_PORT		SHIFT_OUT_ENA_PORT
#define OUT_ENA_CLOCK		CLOCK_ENABLE(SHIFT_OUT_ENA_PIN)
#define SR_CLR_PIN			SHIFT_SER_CLR_PIN //G5
#define SR_CLR_PORT			SHIFT_SER_CLR_PORT
#define SR_CLR_CLOCK		CLOCK_ENABLE(SHIFT_SER_CLR_PORT)

void SN54HC595_init();

void SN54HC595_disable();

void SN54HC595_enable();

unsigned char SN54HC595_get_dev_count(void);

unsigned char SN54HC595_out_bytes(unsigned char* data, unsigned char byte_count);

unsigned char SN54HC595_out_bytes_delay(unsigned char* data,
		unsigned char byte_count, uint32_t delay);

void SN54HC595_out(void);

void SN54HC595_clear(void);


#endif /* SN54HC595_H_ */
