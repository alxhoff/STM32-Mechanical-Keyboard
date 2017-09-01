/*
 * process.h
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include "keyboard.h"
#include "keymap.h"
#include "usb_device.h"


void processBuffer();
void resetBuffer(struct sixKeyBufferTag* buffer_to_reset);
void creatHIDReport();
void sendHIDReport();

#endif /* PROCESS_H_ */
