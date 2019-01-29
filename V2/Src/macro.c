/*
 * macro.c
 *
 *  Created on: Sep 13, 2017
 *      Author: alxhoff
 */

#include "error.h"
#include "send.h"

#include <stdlib.h>
#include <string.h>

typedef struct macro_entry macro_entry_t;

struct macro_entry {
	unsigned char sc;

	char* string;

	macro_entry_t *next;
};

typedef struct macro_table {
	macro_entry_t *head;
	macro_entry_t *tail;

	int count;
} macro_table_t;

macro_table_t macro_dev = { 0 };

unsigned char macro_add_new_entry(char *string, unsigned char sc) {
	macro_entry_t *macro = calloc(1, sizeof(macro_entry_t));
	if(!macro)
		return -ENOMEM;

	macro->string = malloc(sizeof(char) * (strlen(string) + 1));
	if (!macro->string)
		goto string_error;

	strcpy(macro->string, string);
	macro->sc = sc;

	if (!macro_dev.head) {
		macro_dev.head = macro;
		macro_dev.tail = macro;
	} else {
		macro_dev.tail->next = macro;
		macro_dev.tail = macro;
	}

	return 0;

	string_error: free(macro);
	return -ENOMEM;
}

//TODO check this
unsigned char macro_rem_entry(unsigned char sc) {
	macro_entry_t *head = macro_dev.head;
	if (head->sc == sc) {
		//single item list
		if (!head->next) {
			macro_dev.head = NULL;
			macro_dev.tail = NULL;
		} else
			//longer than one item
			macro_dev.head = head->next;
		free(head);
		return 0;
	}

	test:	//if list has more than one item
	if (head->next->sc == sc) {
		if (!head->next->next) 	//is tail
			macro_dev.tail = head;
		free(head->next);
		return 0;
	}

	while (head->next->next) {
		head = head->next;
		goto test;
	}
	return -ENOENT;
}

macro_entry_t* macro_get_sc(unsigned char sc) {
	macro_entry_t *head = macro_dev.head;

	test: if (head->sc == sc)
		return head;

	while (head->next) {
		head = head->next;
		goto test;
	}

	return NULL;
}

unsigned char macro_run_sc(unsigned char sc) {
	macro_entry_t *macro = macro_get_sc(sc);
	if (!macro)
		return -ENOENT;

	send_string(macro->string);

	return 0;
}

//STATES
unsigned char macro_init(void){

	return 0;
}

void macro_enter(void) {

}

void macro_run(void) {

}

void macro_exit(void) {

}

unsigned char macro_set_init(void){

	return 0;
}

void macro_set_enter(void){

}

void macro_set_run(void){

}

void macro_set_exit(void){

}
