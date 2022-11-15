/*
 * error.h
 *
 *  Created on: Sep 21, 2017
 *      Author: alxhoff
 */

#ifndef ERROR_H_
#define ERROR_H_

typedef enum {
	ENOMEM = 1,
	EUSB = 2,
	EBUFF = 3,
	EADC = 4,
	ENOINIT = 5,
	ESEND = 6,
	ENOENT = 7,
	EINVAL = 8,
	EAGAIN = 9,
	EAVAIL = 10,
	EWRITE = 11,
	EBOUNDS = 12,
} err_codes_t;

#endif /* ERROR_H_ */
