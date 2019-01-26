/*
 * lookup.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef LOOKUP_H_
#define LOOKUP_H_

unsigned char lookup_get_key(unsigned char character);
unsigned char lookup_get_mod(unsigned char character);
unsigned char lookup_state_change_key(unsigned char sc);
unsigned char lookup_toggle_key(unsigned char sc);


#endif /* LOOKUP_H_ */
