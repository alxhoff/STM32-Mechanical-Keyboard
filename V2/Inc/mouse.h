/*
 * mouse.h
 *
 *  Created on: Sep 1, 2017
 *      Author: alxhoff
 */

#ifndef MOUSE_H_
#define MOUSE_H_

//CALIBRATION
#define LARGE_RANGE 	100
#define SMALL_RANGE 	84

//X
#define X_MIN			53
#define X_MAX			255
#define X_MID			164

//Y
#define Y_MIN			62
#define Y_MAX			255
#define Y_MID			178

unsigned char mouse_init(void);
unsigned char mouse_run(void);

#endif /* MOUSE_H_ */
