/*
 * list.h
 *
 *  Created on: 21 Jan 2019
 *      Author: alxhoff
 */

#ifndef LIST_H_
#define LIST_H_

typedef struct list_entry list_entry_t;

struct list_entry{
	struct list_entry 	*next, *prev;

	void*				data;
};

#endif /* LIST_H_ */
