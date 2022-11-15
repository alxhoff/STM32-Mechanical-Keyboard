/*
 * lookup.h
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#ifndef LOOKUP_H_
#define LOOKUP_H_

const char* lookup_get_char(unsigned char sc, unsigned char mod);
unsigned char lookup_get_key(unsigned char character);
unsigned char lookup_get_mod(unsigned char character);
unsigned char lookupCheckStateChange(unsigned char sc);
unsigned char lookupCaps(unsigned char sc);

#endif /* LOOKUP_H_ */
