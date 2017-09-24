/*
 * CLI.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "CLI.h"

int8_t state_enter_CLI()
{
	current_keyboard_state = CLI;
	vTaskDelay(200);
	return 0;
}

int8_t state_exit_CLI()
{
	current_keyboard_state = typing;
	return 0;
}

int8_t state_CLI()
{
	//get input until enter is pressed


	//process command

	return 0;
}
