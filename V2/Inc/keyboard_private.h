/*
 * keyboard_private.h
 *
 *  Created on: 23 Jan 2019
 *      Author: alxhoff
 */

#ifndef KEYBOARD_PRIVATE_H_
#define KEYBOARD_PRIVATE_H_

#include "buffers.h"

  /**
   * @typedef scaned_key_t
   * @brief Typedef of scaned_key
   * */
  /**
   * @struct scaned_key
   * @brief Represents a single keyboard key
   *
   * The key structure represents a key at a given moment in time by representing
   * its row and column as well as its current key_code. Does not account for layers
   * as it stores one static key_code which can be dynamically changed.
   *
   * */
typedef struct scaned_key{
	unsigned char row;			/**< Key's row */
	unsigned char col;			/**< Key's col */
} scaned_key_t;

/**
 * @typedef scan_buffer_t
 * @brief Typedef of scan_buffer
 * */
/**
 * @struct scan_buffer
 * @brief Keypress input buffer used to store keystroke input from keyboard.
 *
 * When the keyboard is initially scanned, all keys that were found to be pressed are
 * stored in the keypress buffer before being processed. The key presses are saved as
 * row and col values, not key_codes.
 *
 * */
typedef struct scan_buffer{
	scaned_key_t buf[SCAN_KEY_BUFFER_LENGTH];	/**< Key buffer */
	unsigned char count;		/**< Stores the amount of keys currently within the buffer*/
} scan_buffer_t;

typedef struct key_buffer{
	unsigned char buf[SCAN_KEY_BUFFER_LENGTH];
	unsigned char count;
} key_buffer_t;

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

	const unsigned char keyboard_report_ID;
	const unsigned char media_report_ID;

	//TODO lock this to prepare for multicore
	scan_buffer_t scan_buf;			/**< Buffer where scanned keys are stored before processing */
	six_key_buffer_t prev_buf;
} keyboard_device_t;

#endif /* KEYBOARD_PRIVATE_H_ */
