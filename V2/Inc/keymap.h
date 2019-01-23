/*
 * keymap.h
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#ifndef KEYMAP_H_
#define KEYMAP_H_

#include "HIDClassCommon.h"

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

#define K_ESC			HID_KEYBOARD_SC_ESCAPE
#define K_1				HID_KEYBOARD_SC_1_AND_EXCLAMATION
#define K_2				HID_KEYBOARD_SC_2_AND_AT
#define K_3				HID_KEYBOARD_SC_3_AND_HASHMARK
#define K_4				HID_KEYBOARD_SC_4_AND_DOLLAR
#define K_5				HID_KEYBOARD_SC_5_AND_PERCENTAGE
#define K_6				HID_KEYBOARD_SC_6_AND_CARET
#define K_7				HID_KEYBOARD_SC_7_AND_AMPERSAND
#define K_8				HID_KEYBOARD_SC_8_AND_ASTERISK
#define K_9				HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS
#define K_0				HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS
#define K_MINS			HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE
#define K_EQUAL			HID_KEYBOARD_SC_EQUAL_AND_PLUS
#define K_BSPC			HID_KEYBOARD_SC_BACKSPACE
#define K_DEL			HID_KEYBOARD_SC_DELETE
#define K_TAB			HID_KEYBOARD_SC_TAB
#define K_Q				HID_KEYBOARD_SC_Q
#define K_W				HID_KEYBOARD_SC_W
#define K_E				HID_KEYBOARD_SC_E
#define K_R				HID_KEYBOARD_SC_R
#define K_T				HID_KEYBOARD_SC_T
#define K_Y				HID_KEYBOARD_SC_Y
#define K_U				HID_KEYBOARD_SC_U
#define K_I				HID_KEYBOARD_SC_I
#define K_O				HID_KEYBOARD_SC_O
#define K_P				HID_KEYBOARD_SC_P
#define K_LBRC			HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE
#define K_RBRC			HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE
#define K_ENTER			HID_KEYBOARD_SC_ENTER
#define K_CAPS			HID_KEYBOARD_SC_LOCKING_CAPS_LOCK
#define K_A				HID_KEYBOARD_SC_A
#define K_S				HID_KEYBOARD_SC_S
#define K_D				HID_KEYBOARD_SC_D
#define K_F				HID_KEYBOARD_SC_F
#define K_G				HID_KEYBOARD_SC_G
#define K_H				HID_KEYBOARD_SC_H
#define K_J				HID_KEYBOARD_SC_J
#define K_K				HID_KEYBOARD_SC_K
#define K_L				HID_KEYBOARD_SC_L
#define K_COLON			HID_KEYBOARD_SC_SEMICOLON_AND_COLON
#define K_QUOTE			HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE
#define K_BSLASH		HID_KEYBOARD_SC_BACKSLASH_AND_PIPE
#define K_LSHIFT		HID_KEYBOARD_SC_LEFT_SHIFT
#define K_Z				HID_KEYBOARD_SC_Z
#define K_X				HID_KEYBOARD_SC_X
#define K_C				HID_KEYBOARD_SC_C
#define K_V				HID_KEYBOARD_SC_V
#define K_B				HID_KEYBOARD_SC_B
#define K_N				HID_KEYBOARD_SC_N
#define K_M				HID_KEYBOARD_SC_M
#define K_COMMA			HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN
#define K_DOT			HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN
#define K_SLASH			HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK
#define K_RSHIFT		HID_KEYBOARD_SC_RIGHT_SHIFT
#define K_LCTRL			HID_KEYBOARD_SC_LEFT_CONTROL
#define K_LGUI			HID_KEYBOARD_SC_LEFT_GUI
#define K_LALT			HID_KEYBOARD_SC_LEFT_ALT
#define K_SPACE			HID_KEYBOARD_SC_SPACE
#define K_RALT			HID_KEYBOARD_SC_RIGHT_ALT
#define K_RGUI			HID_KEYBOARD_SC_RIGHT_GUI
#define K_FN
#define K_RCRTL			HID_KEYBOARD_SC_RIGHT_CONTROL
#define K_NO			0
#define K_PGUP			HID_KEYBOARD_SC_PAGE_UP
#define K_PGDWN			HID_KEYBOARD_SC_PAGE_DOWN
#define K_END			HID_KEYBOARD_SC_END
#define K_HOME			HID_KEYBOARD_SC_HOME
#define K_TAB			HID_KEYBOARD_SC_KEYPAD_TAB

//ARROWS
#define K_LEFT			HID_KEYBOARD_SC_LEFT_ARROW
#define K_RIGHT			HID_KEYBOARD_SC_RIGHT_ARROW
#define K_UP			HID_KEYBOARD_SC_UP_ARROW
#define K_DOWN			HID_KEYBOARD_SC_DOWN_ARROW

#define K_LAYER			HID_KEYBOARD_SC_LAYER_FUNCTION
#define K_MACRO_R		HID_KEYBOARD_SC_MACRO_RUN_FUNCTION
#define K_MACRO_S		HID_KEYBOARD_SC_MACRO_SET_FUNCTION
#define K_CLI			HID_KEYBOARD_SC_CLI_FUNCTION

//MEDIA
#define K_PLAY			HID_KEYBOARD_SC_MEDIA_PLAY

//NUMPAD
#define K_NUM1			HID_KEYBOARD_SC_KEYPAD_1_AND_END
#define K_NUM2			HID_KEYBOARD_SC_KEYPAD_2_AND_DOWN_ARROW
#define K_NUM3			HID_KEYBOARD_SC_KEYPAD_3_AND_PAGE_DOWN
#define K_NUM4			HID_KEYBOARD_SC_KEYPAD_4_AND_LEFT_ARROW
#define K_NUM5			HID_KEYBOARD_SC_KEYPAD_5
#define K_NUM6			HID_KEYBOARD_SC_KEYPAD_6_AND_RIGHT_ARROW
#define K_NUM7			HID_KEYBOARD_SC_KEYPAD_7_AND_HOME
#define K_NUM8			HID_KEYBOARD_SC_KEYPAD_8_AND_UP_ARROW
#define K_NUM9			HID_KEYBOARD_SC_KEYPAD_9_AND_PAGE_UP
#define K_NUM0			HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT

//prototypes
unsigned char keymap_list_init(keymap_layer_t *layer);

keymap_layer_t *keymap_layer_init(char* name, unsigned char modifier_key_code);

void keymap_layer_set_key(keymap_layer_t *layer, unsigned char row,
		unsigned char col, unsigned char val);

unsigned char keymap_layer_get_key(keymap_layer_t *layer, unsigned char row,
		unsigned char col);

void keymap_layer_set_name(keymap_layer_t *layer, char* name);

void keymap_layer_set_mod_key_code(keymap_layer_t *layer, unsigned char code);

unsigned char keymap_layer_get_mod_key_code(keymap_layer_t *layer);

unsigned char keymap_layer_get_ID(keymap_layer_t *layer);

keymap_layer_t *keymap_layer_get_next(keymap_layer_t *layer);

int keymap_list_append_layer(keymap_layer_t *layer);

int keymap_list_prepend_layer(keymap_layer_t *layer);

int keymap_list_add_pos(keymap_list_t *list,
		keymap_layer_t *layer, unsigned char pos);

unsigned char keymap_list_get_ID_count(void);

unsigned char keymap_list_get_layer_count(void);

keymap_layer_t *keymap_list_get_current_layer(void);

keymap_layer_t *keymap_list_get_layer_ID(unsigned char ID);

keymap_layer_t *keymap_list_get_layer_mod_key(unsigned char key);


#endif /* KEYMAP_H_ */
