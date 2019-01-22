/*
 * devices.c
 *
 *  Created on: 22 Jan 2019
 *      Author: alxhoff
 */

#include "devices.h"

shift_array_t shift_array =
		{ .dev_count = 2,
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
