/*
 * buffers.h
 *
 *  Created on: 24 Jan 2019
 *      Author: alxhoff
 */

#ifndef BUFFERS_H_
#define BUFFERS_H_

#include "config.h"

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
	unsigned char keys[6];
	unsigned char count;					/**< Stores the number of keys currently within the buffer */
} six_key_buffer_t;

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
	unsigned char med_buf;		/**< Stores the media HID report buffer */
//	unsigned char send_flag;				/**< Flag to indicate if the buffers need to be processed */
} send_buffer_t;

#endif /* BUFFERS_H_ */
