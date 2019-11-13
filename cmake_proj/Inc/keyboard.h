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
signed char keyboard_init(void);

signed char keyboard_scan_matrix(void);

/**
 * @brief Processes the input key buffer
 *
 * Once the keyboard scan is complete and the input key buffer has been
 * populated the buffer must be processed. The keyboard state is first handled,
 * going to state code if needed. The buffer is then processed, sorting keys
 * into either modifier keys, media keys or keyboard keys. Once sorted the
 * key codes or bitmasks are retrieved/set. Normal keyboard button presses
 * are first compared against the last HID report as repeating keys get priority
 * in the the next HID report as they represent keys being held down. Keys not
 * in the previous HID report are shortlisted such that after all keys have been
 * converted into key codes the HID report can be filled with shortlisted keys.
 *
 * @param data global keyboard HID data
 * @param layer_list layer list used to convert key_codes
 * @return 0 on success
 */
signed char keyboard_process_scan_buf(void);

#endif /* KEYBOARD_H_ */
