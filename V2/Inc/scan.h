/**
 * @file scan.h
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Functions used to scan the keyboard for input
 *
 * @section scan_sec Scan
 * As the keyboard is based off of a maxrix design, the key matrix
 * must be scanned for input. There are a number of ways that the
 * keyboard can be scanned and for different reasons.
 */

#ifndef SCAN_H_
#define SCAN_H_

#include "error.h"
#include "keyboard.h"
#include "types.h"

/**
 * @brief Defines how long the microcontroller requires a button to be
 * pressed before registering it as a key press event.
 * */
#define DEBOUNCE_DELAY 20

/**
 * @brief Main scanning function called during normal operation of the
 * keyboard
 *
 * During normal operation of the keyboard (acting as a HID device to
 * the PC) the keyboard must be scanned many times each second to detect
 * key press events. This scanning is done by setting the column pins high
 * and polling the row pins to see if a key, at co-ordinates(column,row),
 * has been pressed. To minimize the number of required GPIO pins a shift
 * register is used to set the column pins. The scann process is as follows:
 * <br> - Keypress buffer is cleared <br>
 * - The fist column GPIO is set hight <br>
 * - Each row GPIO is then polled to detech key press events <br>
 * - Key press events are stored into the key buffer, incrementing the key
 * buffer count <br>
 * - Once each row has been polled the column is set low again and the next
 * column is set high. This is done until all columns have been tested.
 *
 * There is no debouncing in this function as the reports are naturally
 * debounced through the USB HID report delay.
 *
 * @param keyboard_dev Keyboard device that is to be scanned
 * @param HID_reports HID data structure where the key buffer is to be stored
 * @return 0 on success
 * */
int8_t scan_key_matrix(keyboard_device_t* keyboard_dev,
		keyboard_HID_data_t* HID_reports);

/**
 * @brief Gets a single not debounced key input from the keyboad
 *
 * Works in a similar fashion to scan_key_matrix but but the function
 * returns upon finding a pressed key. Retrieves a single key and does
 * not prioritise keys in any way. First found is the one returned.
 * <br>
 * Also retrieves the key code for the key from the bottom most
 * keyboard layer
 *
 * @param keyboard_dev Keyboard device that is to be scanned
 * @param layer_list Layer list to be used to find the key's key code
 * @return The key code of a single key press from the bottom most layer of
 * the keyboard
 * layer
 * */
key_code scan_get_single_key( keyboard_device_t* keyboard_dev,
		keymap_list_t* layer_list );

/**
 * @brief Gets a single not debounced key input from the keyboad with
 * a key modifier
 *
 * Works in a similar fashion to scan_key_matrix but but the function
 * returns upon finding a pressed key. Retrieves a single key and key modifier.
 * Does not prioritise keys in any way. First found is the one returned.
 * <br>
 * Also retrieves the key code for the key from the bottom most
 * keyboard layer
 *
 * @param keyboard_dev Keyboard device that is to be scanned
 * @param layer_list Layer list to be used to find the key's key code
 * @return The key code/key modifier pair of a single key press from the
 * bottom most layer of the keyboard
 * */
key_code_w_mod_t scan_get_single_key_w_mod( keyboard_device_t* keyboard_dev,
		keymap_list_t* layer_list );

/**
 * @brief Gets single character input from the keyboard
 *
 * The keyboard is scanned and a single character input is found
 *
 * @param keyboard_dev Keyboard device that is to be scanned
 * @param layer_list Layer list to be used to find the key's key code
 * @return Single char from keyboard input
 * */
char* scan_get_single_key_char( keyboard_device_t* keyboard_dev,
		keymap_list_t* layer_list );

/**
 * @brief Gets input sequence from keyboard
 *
 * Reads in input from the keyboard until a certain key is pressed
 *
 * @param keyboard_dev Keyboard device that is to be scanned
 * @param layer_list Layer list to be used to find the key's key code
 * @param exit_char Character to be presed to exit the function
 * @return Char pointer to a string representation of the input
 * */
char* scan_get_input_seq( keyboard_device_t* keyboard_dev,
		keymap_list_t* layer_list, char exit_char );

#endif /* SCAN_H_ */
