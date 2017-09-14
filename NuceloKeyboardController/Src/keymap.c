/*
 * keymap.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include "keymap.h"
#include "HIDClassCommon.h"

const layer_init keymap_init0 =
{
		.grid =
				KEYMAP(7, 8, 9, \
					   4, 5, 6, \
					   1, 2, 3, \
					   MACRO_S),
	   .key_code = 0x66,
	   .name = "Initial_layer"
};

const layer_init keymap_init1 =
{
		.grid =
				KEYMAP(a, b, c, \
					   f, e, d, \
					   g, h, i, \
					   LAYER),
		.key_code = HID_KEYBOARD_SC_RIGHT_CONTROL,
		.name = "Second_layer"
};

const layer_init keymap_init2 =
{
		.grid =
				KEYMAP(j, k, k, \
					   m, n, o, \
					   o, q, r, \
					   LAYER),
		.key_code = HID_KEYBOARD_SC_LEFT_GUI,
		.name = "Third_layer"
};

//TODO whichspace names
keymap_err_TypeDef layer_list_init( keymap_list_t* layer_list,
//		uint8_t* grid[KEYBOARD_ROWS][KEYBOARD_COLS], char* layer_name)
		layer_init* initial_layer_to_add )
{

	//TODO bravity check
	layer_list->layer_count = 1;
	layer_list->ID_count = 0;

	keymap_layer* layer = (keymap_layer*) malloc (sizeof(keymap_layer));
	if (layer == NULL)
		return km_init_err;

	//NEW LAYER PROPERTIES ###
	layer->ID =  layer_list_get_ID(layer_list);
	layer->layer_modifier_key_code = initial_layer_to_add->key_code;

	layer->name = (char*) malloc ((strlen(initial_layer_to_add->name)+1)*sizeof(char));
	if (layer->name == NULL)
			return km_init_err;
	strcpy(layer->name, initial_layer_to_add->name);

	memcpy(&layer->grid, &initial_layer_to_add->grid, KEYBOARD_ROWS * KEYBOARD_COLS);
	//########################

	//set new layer as head node
	layer_list->layer_head = (keymap_layer*) layer;
	layer->next = layer_list->layer_head;
	layer->prev = layer_list->layer_head;

	return km_ok;
}

keymap_err_TypeDef layer_list_append_layer( keymap_list_t* layer_list, layer_init* layer_to_add )
{

	//create layer
	keymap_layer *layer = (keymap_layer*) malloc (sizeof(keymap_layer));
	if (layer == NULL)
		return km_init_err;

	//fill layer#######
	layer->layer_modifier_key_code = layer_to_add->key_code;
	layer->ID = layer_list_get_ID(layer_list);
	layer_list->layer_count++;

	layer->name = (char*) malloc ((strlen(layer_to_add->name)+1)*sizeof(char));
	if (layer->name == NULL)
		return km_init_err;
	strcpy(layer->name, layer_to_add->name);

	memcpy(&layer->grid, &layer_to_add->grid, KEYBOARD_ROWS * KEYBOARD_COLS);
	//##################


	//point head's prev to new entry (end of list)
	layer_list->layer_head->prev = (keymap_layer*) layer;

	//find end of list
	keymap_layer* head = layer_list->layer_head;
	while(head->next != layer_list->layer_head)
		head = head->next;

	//add new element to end of list
	head->next = (keymap_layer*) layer;

	layer->next = layer_list->layer_head;
	layer->prev = head;

	return km_ok;
}

//init layer table
//loop through all entries in the layer list and create assosiation paris
//between the key_codes and the layer addresses, done using the ID's
keymap_err_TypeDef layer_table_init ( keymap_list_t* layer_list )
{
	//create empty table
	layer_table* table = (layer_table*) malloc(sizeof(layer_table));
	if(table == NULL)
		return km_table_init_err;

	//no table entries yet
	table->layer_head = NULL;
	table->entry_count = 0;

	//assign table to table list
	layer_list->table = table;

	//loop through all layers in layer list and create entries
	keymap_layer* layer_list_head = layer_list->layer_head;

	//loop until the list has been iterated through once
	uint8_t done = 0;
	while(!done){
		//create layer table entry associating ID's to keycodes
		layer_table_entry* new_entry = (layer_table_entry*) malloc(sizeof(layer_table_entry));
		if(new_entry == NULL)
			return km_table_init_err;

		new_entry->ID = layer_list_head->ID;
		new_entry->key_code = layer_list_head->layer_modifier_key_code;
		new_entry->layer = layer_list_head;
		new_entry->grid = &layer_list_head->grid;
		new_entry->next = NULL;

		//if first entry, set head
		if(table->entry_count == 0){
			table->layer_head = new_entry;
			table->entry_count++;
		}
		else
			layer_table_append( layer_list, new_entry);
		layer_list_head = layer_list_head->next;

		if(layer_list_head == layer_list->layer_head)
			done = 1;
	}

	return km_ok;
}

uint8_t layer_list_get_ID( keymap_list_t* layer_list)
{
	return layer_list->ID_count++;
}

//append layer to layer table with it's corresponding key_code
//needs to find the key_code - ID assosiation and append to table
keymap_err_TypeDef layer_table_append ( keymap_list_t* layer_list, layer_table_entry* layer)
{
	//get the end of the list
	//create entry
	layer_table_entry* last = layer_table_get_last(layer_list);

	last->next = layer;
	layer_list->table->entry_count++;

	return km_ok;
}

//get last table entry from layer table
layer_table_entry* layer_table_get_last ( keymap_list_t* layer_list )
{
	if(layer_list->table->layer_head == NULL)
		return NULL;

	layer_table_entry* table_head = layer_list->table->layer_head;

	while(table_head->next != NULL)
		table_head = table_head->next;

	return table_head;
}

keymap_layer* layer_table_get_layer_w_ID( keymap_list_t* layer_list, uint8_t ID)
{
	//iterate through layer table to match ID and return the layer pointed to
	//head of table
	layer_table_entry* head = layer_list->table->layer_head;

	while(head->ID != ID){
		if(head->next == layer_list->table->layer_head){
			return NULL;
		}
		head = head->next;
	}

	return head->layer;
}

keymap_layer* layer_table_get_current_layer ( keymap_list_t* layer_list )
{
	//get layer from layer table with current layer ID
//	keymap_layer* return_layer = layer_table_get_layer_w_ID(layer_list, layer_list->current_layer);
//	return return_layer;
	return layer_table_get_layer_w_ID(layer_list, layer_list->current_layer);
}

//UNTESTED VVVV
//get an ID number for a layer

//remove last layer from layer_list
keymap_err_TypeDef layer_list_rem_last ( keymap_list_t* layer_list )
{
	keymap_layer* end = (keymap_layer*) layer_list->layer_head->prev;

	//set head's prev to second last
	layer_list->layer_head->prev = (keymap_layer*) end->prev;

	//set second last's next to head
	keymap_layer* second_last =(keymap_layer*)  end->prev;
	second_last = layer_list->layer_head;

	//free old struct
	free(end);

	return km_ok;
}

//find layer in layer list that matches input ID
keymap_layer* layer_list_get_layer_w_ID ( keymap_list_t* layer_list, uint8_t ID )
{
	keymap_layer* head = layer_list->layer_head;
	while(head->ID != ID){
		if(head->next == layer_list->layer_head)
			return NULL;
		head = head->next;
	}

	return head;
}

//get last layer in layer list
keymap_layer* layer_list_get_last ( keymap_list_t* layer_list )
{
	keymap_layer* head = layer_list->layer_head;
	while(head->next != NULL || head->next != layer_list->layer_head){
		head = head->next;
	}

	return head;
}

//remove layer from layer list with ID
keymap_err_TypeDef layer_list_rem_layer_w_ID ( keymap_list_t* layer_list, uint8_t ID )
{
	//find node to be removed
	keymap_layer* rem = layer_list_get_layer_w_ID( layer_list, ID);

	//TODO can this be made simpler?
	keymap_layer* previous_node = (keymap_layer*) rem->prev;
	keymap_layer* next_node = (keymap_layer*) rem->next;
	previous_node->next = next_node;
	next_node->prev = previous_node;

	free(rem);

	return km_ok;
}

//get second last table entry from layer table
layer_table_entry* layer_table_get_second_last (keymap_list_t* layer_list )
{
	layer_table_entry* table_head = layer_list->table->layer_head;
	layer_table_entry* second_last;

	while(table_head->next != NULL){
		second_last = table_head;
		table_head = table_head->next;
	}

	return second_last;
}

//remove last entry from layer table
keymap_err_TypeDef layer_table_rem_last ( keymap_list_t* layer_list )
{
	layer_table_entry* last = layer_table_get_last( layer_list );
	layer_table_entry* new_last = layer_table_get_second_last(layer_list);

	new_last->next = NULL;

	free(last);

	return km_ok;
}

//removes a layer table entry with a certain ID
keymap_err_TypeDef layer_table_rem_layer_w_ID ( keymap_list_t* layer_list, uint8_t ID )
{
	layer_table_entry* next;
	layer_table_entry* prev;

	layer_table_entry* rem = layer_list->table->layer_head;

	while(rem->ID != ID){
		prev = rem;
		rem = rem->next;
	}

	prev->next = rem->next;

	free(rem);

	return km_ok;
}

//gets the ID of a layer table entry given it's layer address
uint8_t layer_table_get_ID_w_layer ( keymap_list_t* layer_list, keymap_layer* layer )
{
	keymap_layer* head = layer_list->layer_head;

	while(head != layer)
		head = head->next;

	return head->ID;
}


