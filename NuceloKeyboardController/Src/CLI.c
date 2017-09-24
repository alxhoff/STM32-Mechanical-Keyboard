/*
 * CLI.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include "CLI.h"
#include "scan.h"
#include "keymap.h"
#include "extern.h"

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
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	do{
		//get a character of input
		char input = scan_get_single_key(keyboard_devs->keyboard,
				keyboard_devs->layer_list);

		//process character

	}while(current_keyboard_state == CLI);
	//process command
	CLI_process_key(input);

	state_exit_CLI();

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	return 0;
}

int8_t CLI_process_line()
{

	return 0;
}

int8_t CLI_process_arrows()
{

	return 0;
}

int8_t CLI_process_character()
{

	return 0;
}

int8_t CLI_process_key(char input)
{
	//possible inputs
	//CLI key -> exit
	if(input == KEY(CLI)){
		current_keyboard_state = typing;
		return 0;
	}
	//enter key -> process line
	if(input == KEY(ENTER)){
		//process line function

		return 0;
	}
	//arrow key -> move cursor
	//right
	if(input == 0x4F){

		return 0;
	}
	//left
	if(input == 0x50){

		return 0;
	}
	//character -> modify screen string
	if( (input >= KEY(A) && input <= KEY(0)) ||		//A-Z + 1-0
			(input >= KEY(SPACE) && input <= KEY(SLASH)) ||	// symbols
			input == KEY(BSPC) || input == KEY(DEL) // backspace + delete
			){



		return 0;
	}

	return 0;
}
