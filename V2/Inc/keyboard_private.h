/*
 * keyboard_private.h
 *
 *  Created on: 23 Jan 2019
 *      Author: alxhoff
 */

#ifndef KEYBOARD_PRIVATE_H_
#define KEYBOARD_PRIVATE_H_

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
 * @typedef keyboard_HID_data_t
 * @brief Typedef of keyboard_HID_data
 * */
/**
 * @struct keyboard_HID_data
 * @brief Stores all the HID data required for the keyboard.
 *
 * Contains all buffers and flags required for the gathering
 * and transmission of the keyboard's HID reports.
 *
 * */
typedef struct keyboard_HID_data{
	unsigned char process_key_buf;

	keyboardHID_t keyboard_report;		/**< Keyboard HID report */
	report_states keyboard_state;		/**< State of the keyboard HID report */

	mediaHID_t media_report;			/**< Media HID report */
	report_states media_state;			/**< State of the media HID report */

	send_buffer_t out_buf;				/**< Buffer populated before the HID report are prepared */

	six_key_buffer_t shortlist_keys;	/**< Buffer storing all shortlisted keys from the keyboard
	 	 	 	 	 	 	 	 	 	 	 scan*/

	unsigned char prev_report_len;			/**< Length of previous HID report */
	unsigned char prev_keys[6];				/**< Previous HID report */

} keyboard_HID_data_t;

/**
 * @typedef keyboard_device_t
 * @brief Typedef of keyboard_device
 * */
/**
 * @struct keyboard_device
 * @brief Stores the GPIO interface information
 *
 * Details the GPIO pins and ports used to access the rows
 * and columns of the keyboard.
 *
 * */
typedef struct keyboard_device{
	uint16_t row_pins[KEYBOARD_ROWS];			/**< GPIO pins used by the keyboard rows */
	GPIO_TypeDef* row_ports[KEYBOARD_ROWS];		/**< GPIO ports used by the keyboard rows */

	const keyboard_report_ID;
	const media_report_ID;

	keypress_buffer_t key_buf;			/**< Buffer where scanned keys are stored before processing */
	six_key_buffer_t prev_buf;
//	uint16_t col_pins[KEYBOARD_COLS];			/**< GPIO pins used by the keyboard cols */
//	GPIO_TypeDef* col_ports[KEYBOARD_COLS];		/**< GPIO ports used by the keyboard cols */
} keyboard_device_t;

#endif /* KEYBOARD_PRIVATE_H_ */
