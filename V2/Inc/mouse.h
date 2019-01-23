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


typedef struct mouse_coord{
	signed char 		x;
	signed char 		y;
} mouse_coord_t;

//mouse HID must always be 3
typedef struct mouse_HID{
	unsigned char 		id;
	unsigned char 		buttons;
	mouse_coord_t		coord;
	signed char 		wheel;
  } mouse_HID_t;

unsigned char mouse_init(void);
unsigned char mouse_run (void);

#endif /* MOUSE_H_ */
