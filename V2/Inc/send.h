/*
 * send.h
 *
 *  Created on: 26 Jan 2019
 *      Author: alxhoff
 */

#ifndef SEND_H_
#define SEND_H_

void send_init(void);
void send_enter(void);
void send_run(void);
void send_exit(void);
void send_string(char* str);
unsigned char send_get_send_buf(void);
unsigned char send_reports(void);

#endif /* SEND_H_ */
