/*
 * CLI.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "config.h"
#include "screen.h"

#include <stdlib.h>
#include <string.h>

typedef struct CLI_data{
	char			**screen_buf;
} CLI_data_t;

CLI_data_t CLI_dev = {0};

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



//TODO dynamic line length

	strcpy(CLI_dev.screen_buf[0], "CLI");

	screen_set_buf(CLI_dev.screen_buf);

	line_error:
	//TODO
	page_error:
			return;
}

void CLI_enter(void){

}

void CLI_run(void){

}

void CLI_exit(void){

}
