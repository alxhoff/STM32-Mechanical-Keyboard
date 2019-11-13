/*
 * list.h
 *
 *  Created on: 21 Jan 2019
 *      Author: alxhoff
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct list_entry list_entry_t;

struct list_entry{
	struct list_entry 	*next, *prev;

	void*				data;
};

#endif /* LINKED_LIST_H_ */
