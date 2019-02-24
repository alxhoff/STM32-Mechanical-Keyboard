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
#include "error.h"

#include <stdlib.h>
#include <string.h>

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

void CLI_handle_input(void){



}

void CLI_run(void){
	if(!CLI_recv_presses())
		CLI_handle_input();
}

void CLI_exit(void){

}
