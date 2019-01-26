/*
 * CLI.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#include <stdint.h>

#include "CLI.h"
#include "screen.h"
#include "scan.h"
#include "keymap.h"
#include "lookup.h"

unsigned char state_enter_CLI()
{
	current_keyboard_state = CLI;
	vTaskDelay(200);
	return 0;
}

unsigned char state_exit_CLI()
{
	current_keyboard_state = typing;
	vTaskDelay(200);
	return 0;
}

unsigned char state_CLI()
{
//	//get input until enter is pressed
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//	key_code_w_mod_t input = {0};
//	do{
//		//get a character of input
//		input = scan_get_single_key_w_mod(keyboard_devs->keyboard,
//				keyboard_devs->layer_list);
//
//		//process character
//		CLI_process_key(input);
//
//	}while(current_keyboard_state == CLI);
//	//process command
//
//	state_exit_CLI();
//
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//	return 0;
}

unsigned char CLI_process_line()
{

	return 0;
}

unsigned char CLI_process_arrows(unsigned char left)
{
	//if (string length - x buff shift) - screen width > 0
				// then move

	/*if(left){
		if(GET_SCREEN->cursor_x > 0 &&
				GET_SCREEN->cursor_x <= (GET_LCD->width - 2)){ //middle
			GET_SCREEN->cursor_x--;
		}else{ //edge
			if(GET_SCREEN->x_buff_shift > 0)
				GET_SCREEN->x_buff_shift--;
		}
	}
	else{ //RIGHT
		if(GET_SCREEN->cursor_x >= 0 &&
				GET_SCREEN->cursor_x < (GET_SCREEN->cols - 2)){ //middle
			GET_SCREEN->cursor_x++;
		}else{ //edge
			//TODO check x_buff_shift?
			if((strlen(keyboard_devs->screen->buffers[0] + GET_SCREEN->x_buff_shift)
							- GET_LCD->width) > 0)
				GET_SCREEN->x_buff_shift++;
		}
	}*/
	return 0;
}

//unsigned char CLI_process_character(char input)
//{
	//insert character at x buff offset
	//copy characters after buffer to temp buffer
	/*uint16_t offset = GET_SCREEN->x_buff_shift + GET_SCREEN->cursor_x;

	char buffer[strlen(GET_SCREEN->buffers[0]) - offset];

	//copy back portion of string into buffer
	strcpy(buffer, GET_SCREEN->buffers[0]+ offset);

	//expand buffer
	realloc(GET_SCREEN->buffers[0], strlen(GET_SCREEN->buffers[0]) + 1);

	if(GET_SCREEN->buffers[0] == NULL) return -ENOMEM;

	//reconstruct buffer
	*(GET_SCREEN->buffers[0] + offset) = input;

	strcpy(GET_SCREEN->buffers[0] + offset + 1, buffer);

	return 0;*/
//}

//unsigned char CLI_process_key(scan_code_t input)
//{
	//possible inputs
	//CLI key -> exit
//	if(input.key_code == KEY(CLI)){
//		current_keyboard_state = typing;
//		return 0;
//	}
//	//enter key -> process line
//	if(input.key_code == KEY(ENTER)){
//		//process line function
//
//		return 0;
//	}
//	//arrow key -> move cursor
//	//right
//	if(input.key_code == 0x4F){
//		CLI_process_arrows(0);
//		return 0;
//	}
//	//left
//	if(input.key_code == 0x50){
//		CLI_process_arrows(1);
//		return 0;
//	}
//	//character -> modify screen string
//	if( (input.key_code >= KEY(A) && input.key_code <= KEY(0)) ||		//A-Z + 1-0
//			(input.key_code >= KEY(SPACE) && input.key_code <= KEY(SLASH))	// symbols
//			){
//		//TODO MODIFIERS MAYBE?
//		if(input.modifier)
//			CLI_process_character(*lookup_char[(uunsigned char)input.key_code].modified);
//		else
//			CLI_process_character(*lookup_char[(uunsigned char)input.key_code].unmodified);
//
//
//		return 0;
//	}
//
//	if( input.key_code == KEY(BSPC) || input.key_code == KEY(DEL) )
//	{
//
//		return 0;
//	}
//
//	return 0;
//}
