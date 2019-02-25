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
} CLI_data_t;

CLI_data_t CLI_dev = {0};
send_buffer_t key_buf = {0};
static SemaphoreHandle_t processing_lock = NULL;

signed char CLI_process_line(void)
{

	return 0;
}


void CLI_init(void){

	CLI_dev.screen_buf = screen_get_buffer();

	if(!CLI_dev.screen_buf)
		return;

	processing_lock = xSemaphoreCreateMutex();
	if(!processing_lock)
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

signed char CLI_write_to_string_front(char **str, char *new){
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

signed char CLI_append_to_string(char **str, char *new){
	size_t str_len = strlen(*str);
#if CLI_DYNAMIC_LINE_LENGTH
	size_t new_len = strlen(new);

	*str = realloc(sizeof(char) * (strlen(str) + new_len + 1));
#else
	if(str_len == CLI_LINE_LENGTH)
		return -EBUFF;
#endif

	strcpy(*str + str_len, new);

	return 0;
}

signed char CLI_add_to_string_at_pos(char **str, char *new, int pos){

	size_t new_len = strlen(new);
	size_t str_len = strlen(*str);

	*str = realloc(*str, sizeof(char) * (str_len + new_len + 1));

	if (!*str)
		return -ENOMEM;

	char *tmp = calloc(sizeof(char), (strlen(*str) - pos + 1));
	if (!tmp)
		return -ENOMEM;

	strcpy(tmp, *str + pos);
	strncpy(*str + pos, new, new_len);
	strcpy(*str + pos + new_len, tmp);
	free(tmp);
}

void CLI_handle_input(void){

	unsigned char mod = (((key_buf.mod_buf >> 1 ) & 1) | ((key_buf.mod_buf >> 5) & 1)) ? 1 : 0;

	for(int i = 0; i < key_buf.key_buf.count; i++){
		CLI_add_to_string_at_pos( &CLI_dev.screen_buf[0],
				(char *)lookup_get_char(key_buf.key_buf.keys[i], mod), screen_get_cursor_x());
		screen_move_cursor_right();
	}

	xSemaphoreGive(processing_lock);
}

void CLI_run(void){
	if(!CLI_recv_presses())
		CLI_handle_input();
}

void CLI_exit(void){

}
