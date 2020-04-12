#ifndef __CONFIG_H__
#define __CONFIG_H__ 

#include "defconfig.h"

#define KEYBOARD_ROWS 5
#define KEYBOARD_COLS 15

#define DEFAULT_KEYMAP keymap_init0

keymap_layer_t keymap_init0 =
{
		.grid =
				KEYMAP(	ESC, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, MINS, EQUAL, BSPC, PGUP, \
						TAB, Q, W, E, R, T, Y, U, I, O, P, LBRC, RBRC, BSLASH, PGDWN, \
						CAPS, A, S, D, F, G, H, J, K, L, COLON, QUOTE, NO, ENTER, HOME, \
						LSHIFT, NO, Z, X, C, V, B, N, M, COMMA, DOT, SLASH, RSHIFT, UP, END, \
						RCRTL, RGUI, RALT, NO, NO, NO, SPACE, NO, NO, NO, LAYER, MACRO_S, LEFT, DOWN, RIGHT),
	   .mod_key_code = 0x66,
	   .name = "Initial_layer"
};

keymap_layer_t keymap_init1 =
{
		.grid =
				KEYMAP(	ESC, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, MINS, EQUAL, BSPC, PGUP, \
				TAB, NUM7, NUM8, NUM9, R, T, Y, U, I, O, P, LBRC, RBRC, BSLASH, PGDWN, \
				CAPS, NUM4, NUM5, NUM6, F, G, H, J, K, L, COLON, QUOTE, NO, ENTER, HOME, \
				LSHIFT, NO, NUM1, NUM2, NUM3, V, B, N, M, COMMA, DOT, SLASH, RSHIFT, UP, END, \
				RCRTL, RGUI, RALT, NO, NO, NO, SPACE, NO, NO, NO, LAYER, MACRO_S, LEFT, DOWN, RIGHT),
		.mod_key_code = HID_KEYBOARD_SC_Q,
		.name = "Second_layer"
};

#endif // __CONFIG_H__
