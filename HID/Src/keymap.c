
#include <stdlib.h>
#include <stdint.h>

#include "keymap.h"
#include "defkeymaps.h"
#include "ll.h"

uint8_t keymap_rows = KEYBOARD_COLS;
uint8_t keymap_cols = KEYBOARD_ROWS;

ll_handle_t keymap_list;

struct keymap *current_keymap = NULL;

uint8_t keymapGetScanCode(uint8_t col, uint8_t row)
{
	if (current_keymap)
		return current_keymap->grid[col][row];

	return HID_KEYBOARD_SC_ERROR_UNDEFINED;
}

int8_t keymapInit(struct keymap *km)
{
	keymap_list = llCreateList(sizeof(struct keymap));
	if (!keymap_list)
		return -1;

	if (llAddItem(keymap_list, &DEFAULT_KEYMAP))
		return -1;

	current_keymap = &DEFAULT_KEYMAP;

	return 0;
}

uint8_t getKeyboardRowCount(void)
{
	return keymap_rows;
}

uint8_t getKeyboardColCount(void)
{
	return keymap_cols;
}

struct keymap *keymapCreate(char *name)
{
	struct keymap *km = (struct keymap *)calloc(1, sizeof(struct keymap));
	if (!km)
		goto err_keymap;

	if (name) {
		km->name = (char *)strdup(name);
		if (!km->name)
			goto err_name;
	}
	llAddItem(keymap_list, (void *)km);

	return km;

err_name:
	free(km);
err_keymap:
	return NULL;
}

struct keymap *keymapFind(char *name)
{
	struct ll_item *iterator = llGetHead(keymap_list);

	for (; iterator->next; iterator = iterator->next)
		if (!strcmp(name, ((struct keymap *)(iterator->data))->name))
			return (struct keymap *)iterator->data;

	return NULL;
}

int8_t keymapDelete(char *name)
{
	struct keymap *km = keymapFind(name);
	if (!km)
		return -1;

	return llDeleteItemData(keymap_list, (void *)km);
}
