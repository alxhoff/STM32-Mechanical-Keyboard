/*
 * keymap.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include "keymap.h"

const key_grid_TypeDef keymap0 =
    /* 0: qwerty */
    KEYMAP(7, 8, 9, \
           4, 5, 6, \
           1, 2, 3, \
		   LGUI);

const key_grid_TypeDef keymap1 =
    /* 0: qwerty */
    KEYMAP(a, b, c, \
           f, e, d, \
           g, h, i, \
		   LGUI);

keymap_err_TypeDef layer_list_init( keymap_list* layer_list,
		key_grid_TypeDef* grid, char* layer_name )
{

	//TODO bravity check
	layer_list->layer_count = 1;
	layer_list->ID_count = 1;

	keymap_layer *layer = (keymap_layer*) malloc (sizeof(keymap_layer));
	if (layer == NULL)
		return km_init_err;

	//NEW LAYER PROPERTIES
	layer->ID =  layer_list_get_ID(layer_list);

	layer->name = (char*) malloc ((strlen(layer_name)+1)*sizeof(char));
	if (layer->name == NULL)
			return km_init_err;
	strcpy(layer->name, layer_name);

	if(sizeof(keymap0) == sizeof(*layer->grid))
		memcpy(layer->grid, grid, KEYBOARD_ROWS * KEYBOARD_COLS);
	else{
		free(layer);
		return km_init_err;
	}

	if(*layer->grid[0][0] != keymap0[0][0]){
		free(layer);
		return km_init_err;
	}

	//set new layer as head node
	layer_list->head = (keymap_layer*) layer;
	layer->next = layer_list->head;
	layer->prev = layer_list->head;

	return km_ok;
}

keymap_err_TypeDef layer_list_append_layer( keymap_list* layer_list,
		key_grid_TypeDef* grid, char* layer_name )
{

	//create layer
	keymap_layer *layer = (keymap_layer*) malloc (sizeof(keymap_layer));
	if (layer == NULL)
		return km_init_err;

	//fill layer
	layer->ID = layer_list_get_ID(layer_list);
	layer_list->layer_count++;

	layer->name = (char*) malloc ((strlen(layer_name)+1)*sizeof(char));
	if (layer->name == NULL)
		return km_init_err;
	strcpy(layer->name, layer_name);

	memcpy(layer->grid, grid, KEYBOARD_ROWS * KEYBOARD_COLS);

	//point head's prev to new entry (end of list)
	layer_list->head->prev = (keymap_layer*) layer;

	//find end of list
	keymap_layer* head = layer_list->head;
	while(head->next != layer_list->head)
		head = head->next;

	//add new element to end of list
	head->next = (keymap_layer*) layer;

	layer->next = layer_list->head;
	layer->prev = head;

	return km_ok;
}

//UNTESTED VVVV
//get an ID number for a layer
uint8_t layer_list_get_ID( keymap_list* layer_list)
{
	return layer_list->ID_count++;
}

//remove last layer from layer_list
keymap_err_TypeDef layer_list_remove_last ( keymap_list* layer_list )
{
	keymap_layer* end = (keymap_layer*) layer_list->head->prev;

	//set head's prev to second last
	layer_list->head->prev = (keymap_layer*) end->prev;

	//set second last's next to head
	keymap_layer* second_last =(keymap_layer*)  end->prev;
	second_last = layer_list->head;

	//free old struct
	free(end);

	return km_ok;
}

//find layer in layer list that matches input ID
keymap_layer* layer_list_find_with_ID ( keymap_list* layer_list, uint8_t ID )
{
	keymap_layer* head = layer_list->head;
	while(head->ID != ID){
		if(head->next == layer_list->head)
			return NULL;
		head = head->next;
	}

	return head;
}

//get last layer in layer list
keymap_layer* layer_list_get_last ( keymap_list* layer_list )
{
	keymap_layer* head = layer_list->head;
	while(head->next != NULL || head->next != layer_list->head){
		head = head->next;
	}

	return head;
}

//get last table entry from layer table
layer_table_entry* layer_table_get_last ( keymap_list* layer_list )
{
	layer_table_entry* table_head = layer_list->table->head;

	while(table_head->next != NULL)
		table_head = table_head->next;

	return table_head;
}

//remove layer from layer list with ID
keymap_err_TypeDef layer_list_remove_with_ID ( keymap_list* layer_list, uint8_t ID )
{
	//find node to be removed
	keymap_layer* rem = layer_list_find_with_ID( layer_list, ID);

	//TODO can this be made simpler?
	keymap_layer* previous_node = (keymap_layer*) rem->prev;
	keymap_layer* next_node = (keymap_layer*) rem->next;
	previous_node->next = next_node;
	next_node->prev = previous_node;

	free(rem);

	return km_ok;
}

//init layer table
keymap_err_TypeDef layer_table_init ( keymap_list* layer_list )
{
	layer_table* table = (layer_table*) malloc(sizeof(layer_table));
	if(table == NULL)
		return km_table_init_err;

	table->head = NULL;
	table->entry_count = 0;

	layer_list->table = table;

	return km_ok;
}

//get second last table entry from layer table
layer_table_entry* layer_table_get_second_last (keymap_list* layer_list )
{
	layer_table_entry* table_head = layer_list->table->head;
	layer_table_entry* second_last;

	while(table_head->next != NULL){
		second_last = table_head;
		table_head = table_head->next;
	}

	return second_last;
}

//append layer to layer table with it's corresponding key_code
keymap_err_TypeDef layer_table_append ( keymap_list* layer_list, keymap_layer* layer, uint8_t key_code )
{
	layer_table_entry* new = (layer_table_entry*) malloc(sizeof(layer_table_entry));
	//create key ID connection
	new->ID = layer->ID;
	new->key_code = key_code;
	new->next = NULL;
	//check if table is empty
	if(layer_list->table->entry_count == 0){
		//create initial node
		layer_list->table->head = new;
		return km_ok;
	}

	//else appednd
	layer_table_entry* last = layer_table_find_last( layer_list );
	last->next = new;
	return km_ok;
}

//remove last entry from layer table
keymap_err_TypeDef layer_table_remove_last ( keymap_list* layer_list )
{
	layer_table_entry* last = layer_table_get_last( layer_list );
	layer_table_entry* new_last = layer_table_get_second_last(layer_list);

	new_last->next = NULL;

	free(last);

	return km_ok;
}

keymap_err_TypeDef layer_table_remove_with_ID ( keymap_list* layer_list, uint8_t ID )
{
	layer_table_entry* next;
	layer_table_entry* prev;

	layer_table_entry* rem = layer_list->table->head;

	while(rem->ID != ID){
		prev = rem;
		rem = rem->next;
	}

	prev->next = rem->next;

	free(rem);

	return km_ok;
}

uint8_t layer_table_get_ID_with_layer ( keymap_list* layer_list, keymap_layer* layer )
{
	keymap_layer* head = layer_list->head;

	while(head != layer)
		head = head->next;

	return head->ID;
}

