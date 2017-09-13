/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include "states.h"

states_err_t state_enter_macro_set()
{

}

states_err_t state_exit_macro_set()
{
	current_keyboard_state = typing;
}

states_err_t state_macro_set()
{

}

states_err_t state_enter_macro_run()
{

}

states_err_t state_exit_macro_run()
{
	current_keyboard_state = typing;
}

states_err_t state_macro_run()
{

}
