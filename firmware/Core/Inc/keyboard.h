/**
 * @file keyboard.h
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Data types and functions for performing classical keyboard functions
 *
 * While the keyboard offers more advanced features, the classical keyboard I/O
 * features of the keyboard are implemented within this file
 *
 * @mainpage Mechanical Keyboard
 * @section intro_sec Introduction
 * The aim of this keyboard firmware is to develop a keyboard/mouse combo with
 * advanced features such as macros, layers, n-key rollover as well as a
 * command line interface directly into the keyboard.
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

extern QueueHandle_t keyboard_send_buffer_queue;

/**
 * @brief Inits the keyboard device and GPIO pins
 *
 * To initialise the keyboard device, this function must be called
 * with the colum GPIO pins and ports, stored in arrays, passed to it.
 * The GPIO are initialised and all default values are set.
 *
 * @param keyboard_devices global keyboard devices struct
 * @param row_ports array of GPIO pointers to the row ports
 * @param row_pins array of GPIO pin values
 * @return 0 on success
 */
signed char keyboardInit(void);

#endif /* KEYBOARD_H_ */

