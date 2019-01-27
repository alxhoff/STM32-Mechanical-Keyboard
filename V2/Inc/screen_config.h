/*
 * screen_config.h
 *
 *  Created on: 27 Jan 2019
 *      Author: alxhoff
 */

#ifndef SCREEN_CONFIG_H_
#define SCREEN_CONFIG_H_

#define SSD1306_I2C_ADDR        0x78

//DEFAULTS
#define SSD1306_I2C_PORT		&hi2c1
#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64
#define SSD1306_BACKGROUND		0
#define SSD1306_CHAR_WIDTH		13
#define SSD1306_LINE_SPACING	20
#define SSD1306_FONT			&Font_11x18

#endif /* SCREEN_CONFIG_H_ */
