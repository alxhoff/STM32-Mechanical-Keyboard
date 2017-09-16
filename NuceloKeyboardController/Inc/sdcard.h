/*
 * sdcard.h
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdio.h>
#include "fatfs.h"

typedef enum{
	sdcard_ok		= 0,
	sdcard_init_err	= -1
} sdcard_err_t;

typedef struct sdcard_dev sdcard_dev_t;

struct sdcard_dev{
	FATFS*	FatFs;

	FIL*	file;

	uint32_t size;
	uint32_t free;

	uint32_t enabled;
};

sdcard_err_t sdcard_init( sdcard_dev_t* device );

#endif /* SDCARD_H_ */
