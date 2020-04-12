
#include <stdlib.h>

#include "keymap.h"
#include "config.h"
#include "ll.h"

uint8_t keymap_rows = KEYBOARD_COLS;
uint8_t keymap_cols = KEYBOARD_ROWS;

ll_handle_t keymap_list;

int keymapInit(struct keymap *km)
{
	keymap_list = llCreateList(sizeof(struct keymap));
	if (!keymap_list)
		return -1;

	if (llAddItem(keymap_list, &DEFAULT_KEYMAP))
		return -1;

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

	return km;

err_name:
	free(km);
err_keymap:
	return NULL;
}

int keymapAdd(struct keymap *km)
{
    return llAddItem(keymap_list, km);
}
