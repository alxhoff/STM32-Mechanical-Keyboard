/*
 * buffers.h
 *
 *  Created on: 24 Jan 2019
 *      Author: alxhoff
 */

#ifndef BUFFERS_H_
#define BUFFERS_H_

#include "config.h"

#define BUFFER_GET_END(BUFF) BUFF.buff[BUFF.count-1]
#define BUFFER_SET_END(BUFF, VALUE) BUFFER_GET_END(BUFF) = VALUE; BUFF.count++

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
typedef struct six_key_buffer {
	unsigned char keys[6];
	unsigned char count; /**< Stores the number of keys currently within the buffer */
} six_key_buffer_t;

typedef struct key_buffer {
	unsigned char buff[SCAN_KEY_BUFFER_LENGTH];
	unsigned char count;
} key_buffer_t;

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
typedef struct send_buffer {
	key_buffer_t key_buff; /**< Stores the keyboard HID report buffer */
	unsigned char mod_buff; /**< Stores the keyboard HID report modifier keys buffer */
	unsigned char med_buff; /**< Stores the media HID report buffer */
//	unsigned char send_flag;				/**< Flag to indicate if the buffers need to be processed */
} send_buffer_t;

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
typedef struct scaned_key {
	unsigned char row; /**< Key's row */
	unsigned char col; /**< Key's col */
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
typedef struct scan_buffer {
	scaned_key_t buff[SCAN_KEY_BUFFER_LENGTH]; /**< Key buffer */
	unsigned char count; /**< Stores the amount of keys currently within the buffer*/
} scan_buffer_t;

typedef struct toggle_keys {
	unsigned char caps;
	unsigned char CLI;
	unsigned char func;

	unsigned char changed;
} toggle_keys_t;

#endif /* BUFFERS_H_ */
