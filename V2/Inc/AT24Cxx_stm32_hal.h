/*
 * AT24C32_stm32_hal.h
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#ifndef AT24CXX_STM32_HAL_H_
#define AT24CXX_STM32_HAL_H_

#include "stm32f4xx_hal.h"

#define STM_I2C_PORT			hi2c2
#define AT24Cxx_BASE_ADDR_R		0xA1
#define AT24Cxx_BASE_ADDR_W		0xA0
#define AT24Cxx_SET_ADDR		0x07
#define AT24Cxx_USED_MODEL		AT24C32

#define AT24Cxx_I2C_TIMOUT		1000

#ifndef I2C_MEMADD_SIZE_8BIT
#define I2C_MEMADD_SIZE_8BIT	0x00000001U
#endif
#ifndef I2c_MEMADD_SIZE_16BIT
#define I2C_MEMADD_SIZE_16BIT   0x00000010U
#endif

//devices
#define AT24C01_MAX_ADDR		0x007F
#define AT24C32_MAX_ADDR		0x0FFF
#define AT24C64_MAX_ADDR		0x1FFF
#define AT24C128_MAX_ADDR		0x3FFF
#define AT24C256_MAX_ADDR		0x7FFF
#define AT24C512_MAX_ADDR		0xFFFF

//page sizes in bytes
#define AT24C01_PG_SIZE			8
#define AT24C32_PG_SIZE			32
#define AT24C64_PG_SIZE			32
#define AT24C128_PG_SIZE		64
#define AT24C256_PG_SIZE		64
#define AT24C512_PG_SIZE		128

//errors
#define ERR_ADD_DEV			-1

typedef enum {
	AT24C01,
	AT24C32,
	AT24C64,
	AT24C128,
	AT24C256,
	AT24C512
} AT24Cxx_model;

typedef enum {
	at_ok 			= 0,
	at_init_err 	= -1,
	at_add_dev_err 	= -2,
	at_get_stat_err = -3,
	at_w_byte_err	= -4,
	at_w_bytes_err 	= -5,
	at_r_byte_err	= -6,
	at_r_bytes_err	= -7
}AT24Cxx_ERR_TypeDef;

typedef struct{
	I2C_HandleTypeDef* dev_port;
	AT24Cxx_model dev_model;

	uint16_t dev_addr;

	void* next_dev;
	void* prev_dev;
} AT24Cxx_device;

typedef struct{
	//Max 8 devices on single I2C port
	AT24Cxx_device* devices[8];
	uint8_t dev_count;
} AT24Cxx_devices;

//prototypes
AT24Cxx_ERR_TypeDef AT24Cxx_init( AT24Cxx_devices* devices, uint8_t init_dev_addr);
AT24Cxx_ERR_TypeDef AT24Cxx_add_dev( AT24Cxx_devices* devices, uint8_t dev_addr);
AT24Cxx_device* AT24Cxx_get_dev( AT24Cxx_devices* devices, uint8_t index);
AT24Cxx_ERR_TypeDef AT24Cxx_write_byte( AT24Cxx_device* dev, uint8_t* ret, uint16_t mem_addr);
AT24Cxx_ERR_TypeDef AT24Cxx_write_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* ret_buf, uint16_t buf_length);
AT24Cxx_ERR_TypeDef AT24Cxx_read_byte( AT24Cxx_device* dev, uint8_t* ret, uint16_t mem_addr);
AT24Cxx_ERR_TypeDef AT24Cxx_read_byte_buffer( AT24Cxx_device* dev, uint16_t mem_addr, uint8_t* ret_buf, uint16_t buf_len);

#endif /* AT24CXX_STM32_HAL_H_ */
