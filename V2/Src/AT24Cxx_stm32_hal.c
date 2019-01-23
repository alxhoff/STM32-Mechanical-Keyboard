/*

 * AT24Cxx_stm32_hal.c
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff


#include "AT24Cxx_stm32_hal.h"
#include <stddef.h>
#include <stdio.h>


uint16_t AT24Cxx_get_max_addr ( AT24Cxx_device* dev )
{
	switch(dev->dev_model){
	case AT24C01:
		return AT24C01_MAX_ADDR;
		break;
	case AT24C32:
		return AT24C32_MAX_ADDR;
		break;
	case AT24C64:
		return AT24C64_MAX_ADDR;
		break;
	case AT24C128:
		return AT24C128_MAX_ADDR;
		break;
	case AT24C256:
		return AT24C256_MAX_ADDR;
		break;
		//not supported yet, will overflow
	case AT24C512:
		return AT24C512_MAX_ADDR;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

uint16_t AT24Cxx_get_pg_size ( AT24Cxx_device* dev )
{
	switch(dev->dev_model){
	case AT24C01:
		return AT24C01_PG_SIZE;
		break;
	case AT24C32:
		return AT24C32_PG_SIZE;
		break;
	case AT24C64:
		return AT24C64_PG_SIZE;
		break;
	case AT24C128:
		return AT24C128_PG_SIZE;
		break;
	case AT24C256:
		return AT24C256_PG_SIZE;
		break;
	case AT24C512:
		//not supported yet, will overflow
		return AT24C512_PG_SIZE;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

AT24Cxx_ERR_TypeDef AT24Cxx_init( AT24Cxx_devices* devices, uint8_t init_dev_addr)
{
	//adds first device to AT32Cxx_devices array
	//furthur devices should be added by calling AT24Cxx_add_dev()
	AT24Cxx_device *at = (AT24Cxx_device*) malloc(sizeof(AT24Cxx_device));
	if ( at == NULL )
		return at_init_err;

	at->dev_addr = init_dev_addr;
	at->next_dev = NULL;
	at->prev_dev = NULL;
	at->dev_model = AT24Cxx_USED_MODEL;
	at->dev_port = &STM_I2C_PORT;
	devices->dev_count = 1;
	devices->devices[devices->dev_count-1] = at;

	return at_ok;
}

AT24Cxx_ERR_TypeDef AT24Cxx_add_dev( AT24Cxx_devices* devices, uint8_t dev_addr)
{
	if(devices->dev_count == 0 || devices->dev_count > 8)
		return at_add_dev_err;

	AT24Cxx_device *at = (AT24Cxx_device*) malloc(sizeof(AT24Cxx_device));
	if ( at == NULL )
		return at_add_dev_err;

	at->dev_addr = dev_addr;
	at->next_dev = NULL;
	at->prev_dev = &devices[devices->dev_count-2];
	at->dev_model = AT24Cxx_USED_MODEL;
	at->dev_port = &STM_I2C_PORT;
	devices->dev_count++;
	devices->devices[devices->dev_count-1] = at;

	//update previous device's next device
	devices->devices[devices->dev_count-2]->next_dev = at;
	return at_ok;
}

AT24Cxx_device* AT24Cxx_get_dev( AT24Cxx_devices* devices, uint8_t index)
{
	return devices->devices[index];
}

AT24Cxx_ERR_TypeDef AT24Cxx_write_byte( AT24Cxx_device* dev, uint8_t* ret, uint16_t mem_addr)
{
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Write(dev->dev_port,
					AT24Cxx_BASE_ADDR_W | (uint16_t) (dev->dev_addr << 1) ,
					(uint16_t) mem_addr, I2C_MEMADD_SIZE_8BIT, ret, 1,
					AT24Cxx_I2C_TIMOUT) != HAL_OK);

		return at_ok;
	}
	return at_w_byte_err;
}

AT24Cxx_ERR_TypeDef AT24Cxx_write_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* ret_buf, uint16_t buf_length)
{
	//TODO checks
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Write(dev->dev_port,
				AT24Cxx_BASE_ADDR_W | (uint16_t) (dev->dev_addr << 1) ,
				(uint16_t) mem_addr, I2C_MEMADD_SIZE_8BIT, ret_buf, buf_length,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
		return at_ok;
	}
	return at_w_bytes_err;
}

AT24Cxx_ERR_TypeDef AT24Cxx_read_byte( AT24Cxx_device* dev, uint8_t* ret, uint16_t mem_addr)
{
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Read(dev->dev_port,
				AT24Cxx_BASE_ADDR_R | (uint16_t) (dev->dev_addr << 1) ,
				(uint16_t) mem_addr, I2C_MEMADD_SIZE_8BIT, ret, 1,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
		return at_ok;
	}
	return at_r_byte_err;
}

AT24Cxx_ERR_TypeDef AT24Cxx_read_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* ret_buf, uint16_t buf_len)
{
	//TODO checks
	if(mem_addr > 0x00 && mem_addr < AT24Cxx_get_max_addr(dev)){
		while(HAL_I2C_Mem_Read(dev->dev_port,
				AT24Cxx_BASE_ADDR_R | (uint16_t) (dev->dev_addr << 1) ,
				(uint16_t) mem_addr, I2C_MEMADD_SIZE_8BIT, ret_buf, 1,
				AT24Cxx_I2C_TIMOUT) != HAL_OK);
		return at_ok;
	}
	return at_r_bytes_err;
}



*/
