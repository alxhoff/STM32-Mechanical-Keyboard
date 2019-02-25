/*
 * screen.h
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
 */

#ifndef SCREEN_H_
#define SCREEN_H_

signed char screen_init(void);
char **screen_get_buffer(void);
int screen_get_cursor_x(void);
int screen_get_cursor_y(void);
signed char screen_add_line(char *line);
void screen_move_cursor_left(void);
void screen_move_cursor_right(void);

#endif /* SCREEN_H_ */
