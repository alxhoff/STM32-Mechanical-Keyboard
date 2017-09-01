/*
 * AT24C32_stm32_hal.h
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#ifndef AT24CXX_STM32_HAL_H_
#define AT24CXX_STM32_HAL_H_

#include "stm32f4xx_hal.h"

#define STM_I2C_PORT		hi2c2
#define AT24Cxx_ADDR		7
#define AT24Cxx_USED_MODEL	AT24C32

#ifndef I2C_MEMADD_SIZE_8BIT
#define I2C_MEMADD_SIZE_8BIT	0x00000001U
#endif
#ifndef I2c_MEMADD_SIZE_8BIT
#define I2C_MEMADD_SIZE_16BIT   0x00000010U
#endif

//errors
#define ERR_ADD_DEV			-1

typedef enum {AT24C32, AT24C64} AT24Cxx_model;

typedef struct{
	I2C_HandleTypeDef* dev_port;
	AT24Cxx_model dev_model;

	uint16_t dev_addr;

	void* next_dev;
} AT24Cxx_device;

typedef struct{
	//Max 8 devices on single I2C port
	AT24Cxx_device* AT24Cxx_dev_pointers[8];
	uint8_t dev_count;
} AT24Cxx_devices;

//prototypes
signed int AT24Cxx_init(uint8_t init_dev_addr);
signed int AT24Cxx_add_dev(uint8_t dev_addr);
AT24Cxx_device* AT24Cxx_get_dev(uint8_t index);
signed int AT24Cxx_write_byte( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t data);
signed int AT24Cxx_write_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* data_buf, uint16_t buf_length);
signed int AT24Cxx_read_byte( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* data);
signed int AT24Cxx_read_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* data_buf, uint16_t buf_len);

#endif /* AT24CXX_STM32_HAL_H_ */
