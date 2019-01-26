/*
 * keymap.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "keymap_private.h"

#define KEY(SC)	K_##SC

//typedef unsigned char key_grid_TypeDef[KEYBOARD_ROWS][KEYBOARD_COLS];

typedef struct keymap_list{
	unsigned char 		ID_count;

	unsigned char 		layer_count;

	keymap_layer_t 		*head;
	keymap_layer_t 		*tail;

	keymap_layer_t 		*current_layer;
} keymap_list_t;

keymap_list_t keymap_layer_list = {0};



//TODO whichspace names\
//init keymap list with initial layer
unsigned char keymap_list_init(keymap_layer_t *layer)
{
	keymap_layer_list.layer_count = 1;

	keymap_layer_list.head = layer;
	keymap_layer_list.tail = layer;
	keymap_layer_list.current_layer = layer;
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

void keymap_set_key(unsigned char row,
		unsigned char col, unsigned char val)
{
	keymap_layer_list.current_layer->grid[row][col] = val;
}

unsigned char keymap_get_key(unsigned char row, unsigned char col)
{
	return keymap_layer_list.current_layer->grid[row][col];
}

void keymap_set_name(char* name)
{
	 keymap_layer_list.current_layer->name = name;
}

char *keymap_get_name(void)
{
	return  keymap_layer_list.current_layer->name;
}

void keymap_set_mod_key_code(unsigned char code)
{
	 keymap_layer_list.current_layer->mod_key_code = code;
}

unsigned char keymap_get_mod_key_code(void)
{
	return  keymap_layer_list.current_layer->mod_key_code;
}

unsigned char keymap_get_ID(void)
{
	return  keymap_layer_list.current_layer->ID;
}

//add layer to end of layer list
int keymap_list_append_layer(keymap_layer_t *layer)
{
	if(!keymap_layer_list.tail)
		return -ENOINIT; //layer list not initialized

	keymap_layer_list.tail->next = layer;
	keymap_layer_list.tail = layer;

	layer->ID = keymap_layer_list.ID_count++;
	keymap_layer_list.layer_count++;

	return 0;
}

int keymap_list_prepend_layer(keymap_layer_t *layer)
{
	if(!keymap_layer_list.head)
			return -ENOINIT; //layer list not initialized

	layer->next = keymap_layer_list.head;

	keymap_layer_list.head = layer;

	layer->ID = keymap_layer_list.ID_count++;
	keymap_layer_list.layer_count++;

	return 0;
}

unsigned char keymap_list_add_pos(keymap_layer_t *layer, unsigned char pos)
{
	//TODO
	return 0;
}

unsigned char keymap_list_get_ID_count(void)
{
	return keymap_layer_list.ID_count;
}

unsigned char keymap_list_get_layer_count(void)
{
	return keymap_layer_list.layer_count;
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




