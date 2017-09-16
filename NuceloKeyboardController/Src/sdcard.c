/*
 * sdcard.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "sdcard.h"
#include "screen.h"


sdcard_err_t sdcard_init( sdcard_dev_t* device )
{

	//Fat filesystem object
	FATFS* FatFS_obj = (FATFS*)malloc(sizeof(FATFS));
	if(FatFS_obj == NULL)
		return sdcard_init_err;

	//file system object
	FIL* file_obj = (FIL*)malloc(sizeof(FIL));
	if(file_obj == NULL)
			return sdcard_init_err;

	device->FatFs = FatFS_obj;
	device->file = file_obj;
	device->size = 0;
	device->free = 0;
	device->enabled = 1;

	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, device->enabled);

	//mount
	if(f_mount(FatFS_obj, "", 1) == FR_OK){
		  screen_display_2_line("fmount", "done");
	}

	return sdcard_ok;
}
