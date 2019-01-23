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
#include <config.h>
#include "stm32f4xx_hal.h"
/* Local includes */
#include "datatypes.h"
#include "states.h"

/**
 * @brief Length of input buffer used when scanning the keyboard
 * */
#define SCAN_KEY_BUFFER_LENGTH		20

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
   * @typedef single_key_t
   * @brief Typedef of single_key
   * */
  /**
   * @struct single_key
   * @brief Represents a single keyboard key
   *
   * The key structure represents a key at a given moment in time by representing
   * its row and column as well as its current key_code. Does not account for layers
   * as it stores one static key_code which can be dynamically changed.
   *
   * */
typedef struct single_key{
	unsigned char row;			/**< Key's row */
	unsigned char col;			/**< Key's col */
	unsigned char key_code;		/**< Ket's key code */
} single_key_t;

/**
 * @typedef keypress_buffer_t
 * @brief Typedef of keypress_buffer
 * */
/**
 * @struct keypress_buffer
 * @brief Keypress input buffer used to store keystroke input from keyboard.
 *
 * When the keyboard is initially scanned, all keys that were found to be pressed are
 * stored in the keypress buffer before being processed. The key presses are saved as
 * row and col values, not key_codes.
 *
 * */
typedef struct keypress_buffer{
	single_key_t buffer[SCAN_KEY_BUFFER_LENGTH];	/**< Key buffer */
	unsigned char index;									/**< Stores the amount of keys currently within
	 	 	 	 	 	 	 	 	 	 	 	 	 	 the buffer*/
} keypress_buffer_t;

/**
 * @typedef six_key_buffer_t
 * @brief Typedef of six_key_buffer
 * */
/**
 * @struct six_key_buffer
 * @brief A six key buffer used to buffer keyboard HID reports
 *
 * A six key buffer is used as an intermediary buffer used between
 * the input buffers and the HID reports.
 *
 * */
typedef struct six_key_buffer{
	single_key_t keys[6];
	unsigned char count;					/**< Stores the number of keys currently within the buffer */
} six_key_buffer_t;

/**
 * @typedef one_key_buffer_t
 * @brief Typedef of one_key_buffer
 * */
/**
 * @struct one_key_buffer
 * @brief Single key buffer
 *
 * Containing only one key, this single key buffer buffers a single
 * key whilst holding a count that says if the key buffer is populated
 * or not.
 *
 * */
typedef struct one_key_buffer{
	single_key_t key;
	unsigned char count;					/**< Indicates if the single key is currently loaded or not.
	 	 	 	 	 	 	 	 	 	 Using a one key buffer without a count can lead to
	 	 	 	 	 	 	 	 	 	 undefined behaviour*/
} one_key_buffer_t;

/**
 * @typedef send_buffer_t
 * @brief Typedef of send_buffer
 * */
/**
 * @struct send_buffer
 * @brief Output buffers for the total keyboard HID data, media and keyboard.
 *
 * The send buffer is used to buffer the final output data for the keyboard's
 * keypresses, media keys and modifier byte. Its readiness for transmission
 * is indicated by the send_flag.
 *
 * */
typedef struct send_buffer{
	six_key_buffer_t key_buf;		/**< Stores the keyboard HID report buffer */
	unsigned char mod_buf;				/**< Stores the keyboard HID report modifier keys buffer */
	one_key_buffer_t med_buf;		/**< Stores the media HID report buffer */
	unsigned char send_flag;				/**< Flag to indicate if the buffers need to be processed */
} send_buffer_t;


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
unsigned char keyboard_init(key_devices_t* keyboard_devices,
		GPIO_TypeDef* row_ports[KEYBOARD_ROWS], uint16_t row_pins[KEYBOARD_ROWS]);

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
unsigned char process_key_buf(keyboard_HID_data_t* data, keymap_list_t* layer_list);

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
unsigned char process_keyboard_flags ( keyboard_HID_data_t* data );

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
unsigned char process_single_key( keymap_list_t* layer_list, unsigned char col, unsigned char row );

/**
* @brief Clears the keyboard HID report
*/
void clear_keyboard_report(  keyboard_HID_data_t* data );

unsigned char keyboard_send_blank(void);

#endif /* KEYBOARD_H_ */
