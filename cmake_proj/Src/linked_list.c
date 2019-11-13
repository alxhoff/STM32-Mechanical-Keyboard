/*
 * list.c
 *
 *  Created on: 21 Jan 2019
 *      Author: alxhoff
 */

#include "linked_list.h"
#include <stddef.h>
#include <stdlib.h>

list_entry_t *init_list_entry(void* data)
{
	list_entry_t *ret = calloc(1, sizeof(list_entry_t));
	if(!ret)
		return NULL;

	if(!data)
		ret->data = data;

	return ret;
}

void add_list_entry(list_entry_t *head, list_entry_t* entry)
{
	if(!head)
	{
		head = entry;
		return;
	}

	while(head->next)
		head = head->next;

	head->next = entry;
	entry->prev = head;
}

list_entry_t *remove_list_entry(list_entry_t *head, list_entry_t *del)
{
	if(del == head)
	{
		free(head);
		return NULL;
	}

	while(head->next != del && head->next)
		head = head->next;

	if(head->next == del)
	{
		if(del->next)
			head->next = del->next;
		else
			head->next = NULL;
		free(del);
	}
	return head;
}

list_entry_t *get_list_entry(list_entry_t *head, void *val, int (*cmp)(void *,void *))
{

test:
	if(!cmp(head->data, val))
		return head;

	if(head->next)
	{
		head = head->next;
		goto test;
	}

	return NULL;
}
