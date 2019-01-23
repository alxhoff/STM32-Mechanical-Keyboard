/*
 * keymap.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include <string.h>
#include <stdlib.h>

#include "keymap.h"
#include "error.h"

typedef struct keymap_list{
	unsigned char 		ID_count;

	unsigned char 		layer_count;

	keymap_layer_t 		*head;
	keymap_layer_t 		*tail;

	keymap_layer_t 		*current_layer;
} keymap_list_t;

keymap_list_t *keymap_layer_list = NULL;



//TODO whichspace names\
//init keymap list with initial layer
unsigned char keymap_list_init(keymap_layer_t *layer)
{
	keymap_layer_list = calloc(1, sizeof(keymap_list_t));
	if(!keymap_layer_list) return -ENOMEM;

	keymap_layer_list->layer_count = 1;
	keymap_layer_list->current_layer = 0;

	keymap_layer_list->head = layer;
	keymap_layer_list->tail = layer;
	keymap_layer_list->current_layer = layer;
	layer->next = NULL;

	return 0;
}

keymap_layer_t *keymap_layer_init(char* name, unsigned char modifier_key_code)
{
	keymap_layer_t *ret = calloc(1, sizeof(keymap_layer_t));
	if(!ret) goto error;

	if(name)
	{
		ret->name = malloc(sizeof(char) * (strlen(name) + 1));
		if(!ret->name) goto name_error;
		{
			free(ret);
			return NULL;
		}

		strcpy(ret->name, name);
	}

	return ret;
	name_error:
		free(ret);
	error:
		return NULL;
}

void keymap_layer_set_key(keymap_layer_t *layer, unsigned char row,
		unsigned char col, unsigned char val)
{
	layer->grid[row][col] = val;
}

unsigned char keymap_layer_get_key(keymap_layer_t *layer, unsigned char row,
		unsigned char col)
{
	return layer->grid[row][col];
}

void keymap_layer_set_name(keymap_layer_t *layer, char* name)
{
	layer->name = name;
}

char *keymap_layer_get_name(keymap_layer_t *layer)
{
	return layer->name;
}

void keymap_layer_set_mod_key_code(keymap_layer_t *layer, unsigned char code)
{
	layer->mod_key_code = code;
}

unsigned char keymap_layer_get_mod_key_code(keymap_layer_t *layer)
{
	return layer->mod_key_code;
}

unsigned char keymap_layer_get_ID(keymap_layer_t *layer)
{
	return layer->ID;
}

keymap_layer_t *keymap_layer_get_next(keymap_layer_t *layer)
{
	return layer->next;
}

//add layer to end of layer list
int keymap_list_append_layer(keymap_layer_t *layer)
{
	if(!keymap_layer_list->tail)
		return -ENOINIT; //layer list not initialized

	keymap_layer_list->tail->next = layer;
	keymap_layer_list->tail = layer;

	layer->ID = keymap_layer_list->ID_count++;
	keymap_layer_list->layer_count++;

	return 0;
}

int keymap_list_prepend_layer(keymap_layer_t *layer)
{
	if(!keymap_layer_list->head)
			return -ENOINIT; //layer list not initialized

	layer->next = keymap_layer_list->head;

	keymap_layer_list->head = layer;

	layer->ID = keymap_layer_list->ID_count++;
	keymap_layer_list->layer_count++;

	return 0;
}

int keymap_list_add_pos(keymap_layer_t *layer, unsigned char pos)
{
	//TODO
}

unsigned char keymap_list_get_ID_count(void)
{
	return keymap_layer_list->ID_count;
}

unsigned char keymap_list_get_layer_count(void)
{
	return keymap_layer_list->layer_count;
}

keymap_layer_t *keymap_list_get_current_layer(void)
{
	return keymap_layer_list->current_layer;
}

keymap_layer_t *keymap_list_get_layer_ID(unsigned char ID)
{
	if(!keymap_layer_list->head)
		return NULL; //layer list not initialized

	keymap_layer_t *head = keymap_layer_list->head;

test:
	if(head->ID == ID)
		return head;

	while(head->next){
		head = head->next;
		goto test;
	}

	return NULL;
}

keymap_layer_t *keymap_list_get_layer_mod_key(unsigned char key)
{
	if(!keymap_layer_list->head)
		return NULL; //layer list not initialized

	keymap_layer_t* head = keymap_layer_list->head;

test:
	if(head->mod_key_code == key)
		return head;

	while(head->next){
		head = head->next;
		goto test;
	}

	return NULL;
}

//TODO
//remove last layer from layer_list
unsigned  char keymap_list_rem_last ( keymap_list_t* layer_list )
{
	return 0;
}

//remove layer from layer list with ID
unsigned char keymap_list_rem_ID ( keymap_list_t* layer_list, unsigned char ID )
{
	return 0;
}




