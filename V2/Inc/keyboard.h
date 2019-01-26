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

/* -- Includes -- */
/* HAL libraries */
#include "config.h"
#include "stm32f4xx_hal.h"
/* Local includes */
#include "datatypes.h"
#include "states.h"
#include "buffers.h"


typedef enum{
	keyboard,
	media,
	mouse
} report_type;

/**
 * @typedef keyboardHID_t
 * @brief Typedef of keyboardHID
 * */
/**
 * @struct keyboardHID
 * @brief Keyboard HID report
 *
 * The keyboard HID report is the USB HID report sent to handle all keyboard press
 * events, its structure is defined by the HID descriptor.
 *
 * */
typedef struct keyboardHID{
      unsigned char id; 			/**< HID report ID for the keyboard report must always be 1, as described
      	  	  	  	  	  	in the HID descriptor*/
      unsigned char modifiers; 	/**< Byte that send key modifiers. See HIDClassCommon.h for bit details*/
      /** @defgroup HID_keys HID keys
       * @brief Each key byte represents a keypress to be sent via USB
       *  @{
       */
      unsigned char key1;
      unsigned char key2;
      unsigned char key3;
      unsigned char key4;
      unsigned char key5;
      unsigned char key6;
      /** @} */ // end of HID_keys
  }keyboardHID_t;

  /**
   * @typedef mediaHID_t
   * @brief Typedef of mediaHIDg
   * */
  /**
   * @struct mediaHIDg
   * @brief Media HID report
   *
   * The media HID report is the USB HID report sent to handle all media key press
   * events, its structure is defined by the HID descriptor.
   *
   * */
typedef struct mediaHID{
    unsigned char id; 			/**< HID report ID for the media report must be 2, as described in
    						the HID descriptor*/
    unsigned char keys;			/**< Single media key contained in the media HID report */
  } mediaHID_t;

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
void keyboard_init(void);

unsigned char keyboard_scan_matrix(void);

void keyboard_init_status_LEDS(void);

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
unsigned char keyboard_process_scan_buf(void);

/**
* @brief Processes any pending HID reports
*
* Once a HID report is pending the keyboard state/media state will be set to
* active, if the state is such then the current pending report is sent.
* If there is not report pending but one was just sent and empty report is sent.
* This corresponds to the state "clearing".
*
* @param data global keyboard HID data
* @return 0 on success
*/
//unsigned char process_keyboard_flags ( keyboard_HID_data_t* data );

/**
* @brief Retrieves the key code for a single key.
*
* The key code retrieved is taken from the most underlying layer.
*
* @param layer_list layer list to be used to extracting the key code
* @param col column of the key
* @param row row of the key
* @return key code of the key from the most underlying layer
*/
//unsigned char process_single_key( keymap_list_t* layer_list, unsigned char col, unsigned char row );

/**
* @brief Clears the keyboard HID report
*/
//void clear_keyboard_report(  keyboard_HID_data_t* data );
unsigned char keyboard_read_row(unsigned char row);
void keyboard_reset_scan_buff(void);
void keyboard_scan_buff_add(unsigned char col, unsigned char row);
unsigned char keyboard_send_blank(void);

#endif /* KEYBOARD_H_ */
