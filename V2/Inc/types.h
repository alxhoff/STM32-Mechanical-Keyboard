/*
 * types.h
 *
 *  Created on: Sep 21, 2017
 *      Author: alxhoff
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef uint8_t key_code;

typedef struct key_code_w_mod{
	uint8_t key_code;
	uint8_t modifier;
} key_code_w_mod_t;

typedef enum{
	keyboard,
	media,
	mouse
} report_type;

#endif /* TYPES_H_ */
