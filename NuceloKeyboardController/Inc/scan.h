/*
 * scan.h
 *
 *  Created on: Aug 15, 2017
 *      Author: alxhoff
 */

#ifndef SCAN_H_
#define SCAN_H_

#include "keymap.h"

typedef uint8_t key_code;
typedef struct keymap_list keymap_list_t;

key_code scan_get_single_key( keymap_list_t* layer_list );

#endif /* SCAN_H_ */
