/*
 * CLI.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "config.h"
#include "screen.h"
#include "keyboard.h"
#include "cmsis_os.h"
#include "buffers.h"
#include "pipes.h"
#include "lookup.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

//typedef struct{
//	char	*buf;
//	int 	pos;
//} CLI_input_buf;

typedef struct CLI_data{
	char			**screen_buf;
	unsigned char 	cursor_x_pos;
	unsigned char	cursor_y_pos;
} CLI_data_t;

CLI_data_t CLI_dev = {0};
send_buffer_t key_buf = {0};
static SemaphoreHandle_t processing_lock = NULL;

signed char CLI_process_line(void)
{

	return 0;
}


void CLI_init(void){
#if CLI_DYNAMIC_LINE_LENGTH
#else
	CLI_dev.screen_buf = malloc(sizeof(char *) * CLI_HISTORY_LENGTH);
#endif
	if (!CLI_dev.screen_buf)
		goto page_error;

#if CLI_DYNAMIC_LINE_LENGTH

#else
	for (int i = 0; i < CLI_HISTORY_LENGTH; i++){
		CLI_dev.screen_buf[i] = calloc(CLI_LINE_LENGTH, sizeof(char));
		if (!CLI_dev.screen_buf[i])
			goto line_error;
	}
#endif

	processing_lock = xSemaphoreCreateMutex();
	if(!processing_lock)
		return;

//TODO dynamic line length

	strcpy(CLI_dev.screen_buf[0], "CLI");

	screen_set_buf(CLI_dev.screen_buf);

	line_error:
	//TODO
	page_error:
			return;
}

signed char CLI_recv_presses(void){
	unsigned char ret = 0;
	if(queue_packet_to_send)
		if(xSemaphoreTake(processing_lock, (TickType_t) 0) == pdTRUE ){
			ret = xQueueReceive(queue_packet_to_send, &key_buf, (TickType_t) portMAX_DELAY);
			if(ret == pdTRUE)
				return 0;
			goto error;
		}
error:
	xSemaphoreGive(processing_lock);
	return -ENOINIT;
}

void CLI_enter(void){

}

signed char CLI_write_to_cursor(char **str, char *new){
	size_t new_len = strlen(new);
#if CLI_DYNAMIC_LINE_LENGTH
	char *dest = malloc(sizeof(char) * (strlen(str) + new_len + 1));
#else
	volatile char *dest = calloc(CLI_LINE_LENGTH, sizeof(char));
#endif

	if (!dest)
		return -ENOMEM;

	strcpy((char *)dest + new_len, *str);
	strncpy((char *)dest, new, new_len);

	//set new string to buffer line and free old string
	free(*str);
	*str = (char *)dest;

	return 0;
}

void CLI_handle_input(void){
	unsigned char mod = (((key_buf.mod_buf >> 1 ) & 1) | ((key_buf.mod_buf >> 5) & 1)) ? 1 : 0;
	for(int i = 0; i < key_buf.key_buf.count; i++)
		CLI_write_to_cursor( &CLI_dev.screen_buf[0], (char *)lookup_get_char(key_buf.key_buf.keys[i], mod));


	xSemaphoreGive(processing_lock);
}

void CLI_run(void){
	if(!CLI_recv_presses())
		CLI_handle_input();
}

void CLI_exit(void){

}
