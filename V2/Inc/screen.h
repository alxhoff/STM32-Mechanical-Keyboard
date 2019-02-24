/*
 * screen.h
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#ifndef SCREEN_H_
#define SCREEN_H_

unsigned char screen_init(void);
void screen_add_line(char *line);
void screen_set_buf(char **buf);

#endif /* SCREEN_H_ */
