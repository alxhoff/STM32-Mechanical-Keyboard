
#include "stm32f4xx_hal.h"
#include "fonts.h"

//	Deze library is door Olivier Van den Eede 2016 geschreven en aangepast voor gebruik met
//	Stm32 microcontrollers en maakt gebruik van de HAL-i2c library's.
//
//	Deze library is gemaakt om gebruik te kunnen maken van een ssd1306 gestuurd oled display.
//	Voor het gebruik moeten zeker de onderstaande defines juist ingesteld worden.
//	Zoals de gebruikte i2c poort en de groote van het scherm.
//
//	De library maakt gebruik van 2 files (fonts.c/h) Hierin staan 3 fonts beschreven.
//	Deze fonts kunnen zo gebruikt worden: 	- Font_7x10
//											- Font_11x18
//											- Font_16x26

#ifndef ssd1306
#define ssd1306

#define SSD1306_I2C_ADDR        0x78

//DEFAULTS
#define SSD1306_I2C_PORT		hi2c1
#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64
#define SSD1306_BACKGROUND		0

#define GET_LCD keyboard_devs->LCD
#define LCD_CLEAR keyboard_devs->LCD->clear(keyboard_devs->LCD)
#define LCD_SET_CURSOR(x,y) keyboard_devs->LCD->cursor(keyboard_devs->LCD, x, y)
#define LCD_WRITE_STRING(input) keyboard_devs->LCD->string(keyboard_devs->LCD, input)
#define LCD_UPDATE keyboard_devs->LCD->update(keyboard_devs->LCD)

typedef enum {
	Black = 0x00, /*!< Black color, no pixel */
	White = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_colour_t;

typedef struct SSD1306_device SSD1306_device_t;
struct SSD1306_device{
	uint16_t x;
	uint16_t y;

	uint8_t inverted;
	uint8_t initialized;

	SSD1306_colour_t background;

	FontDef* font;

	uint8_t width;
	uint8_t height;

	uint8_t buffer [SSD1306_WIDTH * SSD1306_HEIGHT / 8];

	I2C_HandleTypeDef* port;

	HAL_StatusTypeDef (*command)(SSD1306_device_t*, uint8_t);
	HAL_StatusTypeDef (*clear)(SSD1306_device_t*);
	HAL_StatusTypeDef (*update)(SSD1306_device_t*);
	HAL_StatusTypeDef (*fill)(SSD1306_device_t*, SSD1306_colour_t);
	HAL_StatusTypeDef (*string)(SSD1306_device_t*, char*);
	void (*cursor)(SSD1306_device_t*, uint8_t, uint8_t);
};

typedef struct SSD1306_device_init{
	SSD1306_colour_t background;

	FontDef* font;

	uint8_t width;
	uint8_t height;

	I2C_HandleTypeDef* port;
}SSD1306_device_init_t;

SSD1306_device_t* ssd1306_init(SSD1306_device_init_t* init_dev_vals);
HAL_StatusTypeDef ssd1306_fill(SSD1306_device_t* self, SSD1306_colour_t color);
HAL_StatusTypeDef ssd1306_update_screen(SSD1306_device_t* self);
HAL_StatusTypeDef ssd1306_clear(SSD1306_device_t* self);
HAL_StatusTypeDef ssd1306_write_command(SSD1306_device_t* self, uint8_t command);
void ssd1306_draw_pixel(SSD1306_device_t* self, uint8_t x, uint8_t y, SSD1306_colour_t colour);
char ssd1306_write_char(SSD1306_device_t* self, char ch, FontDef Font, SSD1306_colour_t color);
char ssd1306_write_string(SSD1306_device_t* self, char* str);
void ssd1306_set_cursor(SSD1306_device_t* self, uint8_t x, uint8_t y);

#endif
