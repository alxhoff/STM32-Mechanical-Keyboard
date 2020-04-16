/*
 * keymap.h
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#ifndef KEYMAP_H_
#define KEYMAP_H_

#include <stdint.h>

#include "HIDClassCommon.h"
#include "config.h"
#include "keymap_config.h"

#define KEYMAP(K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11,     \
	       K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23,     \
	       K24, K25, K26, K27, K28, K29, K30, K31, K32, K33, K34, K35,     \
	       K36, K37, K38, K39, K40, K41, K42, K43, K44, K45, K46, K47,     \
	       K48, K49, K50, K51, K52, K53, K54, K55, K56, K57, K58, K59,     \
	       K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K70, K71,     \
	       K72, K73, K74)                                                  \
	{                                                                      \
		{ HID_KEYBOARD_SC_##K00, HID_KEYBOARD_SC_##K01,                \
		  HID_KEYBOARD_SC_##K02, HID_KEYBOARD_SC_##K03,                \
		  HID_KEYBOARD_SC_##K04, HID_KEYBOARD_SC_##K05,                \
		  HID_KEYBOARD_SC_##K06, HID_KEYBOARD_SC_##K07,                \
		  HID_KEYBOARD_SC_##K08, HID_KEYBOARD_SC_##K09,                \
		  HID_KEYBOARD_SC_##K10, HID_KEYBOARD_SC_##K11,                \
		  HID_KEYBOARD_SC_##K12, HID_KEYBOARD_SC_##K13,                \
		  HID_KEYBOARD_SC_##K14 },                                     \
			{ HID_KEYBOARD_SC_##K15, HID_KEYBOARD_SC_##K16,        \
			  HID_KEYBOARD_SC_##K17, HID_KEYBOARD_SC_##K18,        \
			  HID_KEYBOARD_SC_##K19, HID_KEYBOARD_SC_##K20,        \
			  HID_KEYBOARD_SC_##K21, HID_KEYBOARD_SC_##K22,        \
			  HID_KEYBOARD_SC_##K23, HID_KEYBOARD_SC_##K24,        \
			  HID_KEYBOARD_SC_##K25, HID_KEYBOARD_SC_##K26,        \
			  HID_KEYBOARD_SC_##K27, HID_KEYBOARD_SC_##K28,        \
			  HID_KEYBOARD_SC_##K29 },                             \
			{ HID_KEYBOARD_SC_##K30, HID_KEYBOARD_SC_##K31,        \
			  HID_KEYBOARD_SC_##K32, HID_KEYBOARD_SC_##K33,        \
			  HID_KEYBOARD_SC_##K34, HID_KEYBOARD_SC_##K35,        \
			  HID_KEYBOARD_SC_##K36, HID_KEYBOARD_SC_##K37,        \
			  HID_KEYBOARD_SC_##K38, HID_KEYBOARD_SC_##K39,        \
			  HID_KEYBOARD_SC_##K40, HID_KEYBOARD_SC_##K41,        \
			  HID_KEYBOARD_SC_##K42, HID_KEYBOARD_SC_##K43,        \
			  HID_KEYBOARD_SC_##K44 },                             \
			{ HID_KEYBOARD_SC_##K45, HID_KEYBOARD_SC_##K46,        \
			  HID_KEYBOARD_SC_##K47, HID_KEYBOARD_SC_##K48,        \
			  HID_KEYBOARD_SC_##K49, HID_KEYBOARD_SC_##K50,        \
			  HID_KEYBOARD_SC_##K51, HID_KEYBOARD_SC_##K52,        \
			  HID_KEYBOARD_SC_##K53, HID_KEYBOARD_SC_##K54,        \
			  HID_KEYBOARD_SC_##K55, HID_KEYBOARD_SC_##K56,        \
			  HID_KEYBOARD_SC_##K57, HID_KEYBOARD_SC_##K58,        \
			  HID_KEYBOARD_SC_##K59 },                             \
		{                                                              \
			HID_KEYBOARD_SC_##K60, HID_KEYBOARD_SC_##K61,          \
				HID_KEYBOARD_SC_##K62, HID_KEYBOARD_SC_##K63,  \
				HID_KEYBOARD_SC_##K64, HID_KEYBOARD_SC_##K65,  \
				HID_KEYBOARD_SC_##K66, HID_KEYBOARD_SC_##K67,  \
				HID_KEYBOARD_SC_##K68, HID_KEYBOARD_SC_##K69,  \
				HID_KEYBOARD_SC_##K70, HID_KEYBOARD_SC_##K71,  \
				HID_KEYBOARD_SC_##K72, HID_KEYBOARD_SC_##K73,  \
				HID_KEYBOARD_SC_##K74                          \
		}                                                              \
	}

enum e_keymap_type {
	keymap_layer,
	keymap_overlay,
};

struct keymap {
	uint8_t grid[KEYBOARD_COLS][KEYBOARD_ROWS];
	enum e_keymap_type type;
	uint8_t modifier;
	char *name;
};

uint8_t getKeyboardRowCount(void);
uint8_t getKeyboardColCount(void);

int8_t keymapInit(struct keymap *km);
struct keymap *keymapCreate(char *name);
int8_t keymapDelete(char *name);

uint8_t keymapGetScanCode(uint8_t col, uint8_t row);

#endif /* KEYMAP_H_ */
