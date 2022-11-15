/*
 * keymap.c
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#include "keymap_defaults.h"

#define KEY(SC)	K_##SC

struct keymap_list {
	struct list_item list;

	unsigned char layer_count;
	keymap_layer_t *current_layer;
} keymap_layer_list = { 0 };

static inline void keymapAddLayer(keymap_layer_t layer) {
	ll_add(&layer.list, &keymap_layer_list.list);

	if (keymap_layer_list.current_layer == NULL)
		keymap_layer_list.current_layer = &layer;
}

static inline void keymapAddLayers(void) {
	for (int i = 0; i < no_default_keymap_layers; i++) {
		keymapAddLayer(default_keymap_layers[i]);
		keymap_layer_list.layer_count++;
	}
}

void keymapInit(void) {
	keymapAddLayers();
}

void keymapSetKeyCode(unsigned char row, unsigned char col, unsigned char val) {
	keymap_layer_list.current_layer->grid[row][col] = val;
}

unsigned char keymapGetKeyCode(unsigned char row, unsigned char col) {
	return keymap_layer_list.current_layer->grid[row][col];
}

void keymapSetCurrentName(char *name) {
	keymap_layer_list.current_layer->name = name;
}

char* keymapGetCurrentName(void) {
	return keymap_layer_list.current_layer->name;
}

void keymapSetCurrentModKeyCode(unsigned char code) {
	keymap_layer_list.current_layer->mod_key_code = code;
}

unsigned char keymapGetCurrentModKeyCode(void) {
	return keymap_layer_list.current_layer->mod_key_code;
}

unsigned char keymapGetCurrentID(void) {
	return keymap_layer_list.current_layer->ID;
}

unsigned char keymapGetLayerCount(void) {
	return keymap_layer_list.layer_count;
}
