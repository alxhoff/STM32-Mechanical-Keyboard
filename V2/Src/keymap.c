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


#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12, K13, K14, 	\
    K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, 	\
    K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K40, K41, K42, K43, K44, 	\
    K45, K46, K47, K48, K49, K50, K51, K52, K53, K54, K55, K56, K57, K58, K59, 	\
	K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K70, K71, K72, K73, K74	\
) { \
    { K_##K00, K_##K01, K_##K02, K_##K03, K_##K04, K_##K05, K_##K06, K_##K07, K_##K08, K_##K09, K_##K10, K_##K11, K_##K12, K_##K13, K_##K14}, \
    { K_##K15, K_##K16, K_##K17, K_##K18, K_##K19, K_##K20, K_##K21, K_##K22, K_##K23, K_##K24, K_##K25, K_##K26, K_##K27, K_##K28, K_##K29}, \
    { K_##K30, K_##K31, K_##K32, K_##K33, K_##K34, K_##K35, K_##K36, K_##K37, K_##K38, K_##K39, K_##K40, K_##K41, K_##K42, K_##K43, K_##K44}, \
    { K_##K45, K_##K46, K_##K47, K_##K48, K_##K49, K_##K50, K_##K51, K_##K52, K_##K53, K_##K54, K_##K55, K_##K56, K_##K57, K_##K58, K_##K59}, \
    { K_##K60, K_##K61, K_##K62, K_##K63, K_##K64, K_##K65, K_##K66, K_##K67, K_##K68, K_##K69, K_##K70, K_##K71, K_##K72, K_##K73, K_##K74}  \
}

#define KEY(SC)	K_##SC

typedef unsigned char key_grid_TypeDef[KEYBOARD_ROWS][KEYBOARD_COLS];

typedef struct keymap_layer keymap_layer_t;

struct keymap_layer{
//	key_grid_TypeDef* grid;
	unsigned char 		grid[KEYBOARD_ROWS][KEYBOARD_COLS];

	unsigned char 		mod_key_code;

	char* 				name;
	unsigned char 		ID;

	keymap_layer_t* 	next;
};

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

unsigned char keymap_get_key_sk(scaned_key_t sk)
{
	return keymap_get_key(sk.row, sk.col);
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

int keymap_list_add_pos(keymap_layer_t *layer, unsigned char pos)
{
	//TODO
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




