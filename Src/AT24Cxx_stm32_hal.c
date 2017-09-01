/*
 * AT24Cxx_stm32_hal.c
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#include "AT24Cxx_stm32_hal.h"
#include <stddef.h>
#include <stdio.h>

//include for i2c handle
//**************************
#include "extern.h"
//**************************

AT24Cxx_devices AT24Cxx_active_devices;

signed int AT24Cxx_init(uint8_t init_dev_addr)
{
	//adds first device to AT32Cxx_devices array
	//furthur devices should be added by calling AT24Cxx_add_dev()
	AT24Cxx_device *at = malloc(sizeof(AT24Cxx_device));
	at->dev_addr = init_dev_addr;
	at->next_dev = NULL;
	at->dev_model = AT24Cxx_USED_MODEL;
	at->dev_port = &STM_I2C_PORT;
	AT24Cxx_active_devices.dev_count = 1;
	AT24Cxx_active_devices.AT24Cxx_dev_pointers[AT24Cxx_active_devices.dev_count-1] = at;

	return 0;
}

signed int AT24Cxx_add_dev(uint8_t dev_addr)
{
	if(AT24Cxx_active_devices.dev_count == 0 || AT24Cxx_active_devices.dev_count > 8)
		return ERR_ADD_DEV;

	AT24Cxx_device *at = malloc(sizeof(AT24Cxx_device));
	at->dev_addr = dev_addr;
	at->next_dev = NULL;
	at->dev_model = AT24Cxx_USED_MODEL;
	at->dev_port = &STM_I2C_PORT;
	AT24Cxx_active_devices.dev_count++;
	AT24Cxx_active_devices.AT24Cxx_dev_pointers[AT24Cxx_active_devices.dev_count-1] = at;

	AT24Cxx_active_devices.AT24Cxx_dev_pointers[AT24Cxx_active_devices.dev_count-2]->next_dev =
			at;
	return 0;
}

AT24Cxx_device* AT24Cxx_get_dev(uint8_t index)
{
	return AT24Cxx_active_devices.AT24Cxx_dev_pointers[index];
}

signed int AT24Cxx_write_byte( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t data)
{
	HAL_I2C_Mem_Write(dev->dev_port,
			(dev->dev_addr << 1) | 0xA0,
			mem_addr, I2C_MEMADD_SIZE_16BIT, &data, 1, 10);

	return 0;
}

signed int AT24Cxx_write_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* data_buf, uint16_t buf_length)
{
	HAL_I2C_Mem_Write(dev->dev_port,
			(dev->dev_addr << 1) | 0xA0,
			mem_addr, I2C_MEMADD_SIZE_16BIT, data_buf, buf_length, 10);

	return 0;
}

signed int AT24Cxx_read_byte( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* data)
{
	HAL_I2C_Mem_Read(dev->dev_port,
			(dev->dev_addr << 1) | 0xA0,
			mem_addr, I2C_MEMADD_SIZE_16BIT, data, 1, 10);

	return 0;
}

signed int AT24Cxx_read_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* data_buf, uint16_t buf_len)
{
	HAL_I2C_Mem_Read(dev->dev_port,
			(dev->dev_addr << 1) | 0xA0,
			mem_addr, I2C_MEMADD_SIZE_16BIT, data_buf, 1, 10);

	return 0;
}



