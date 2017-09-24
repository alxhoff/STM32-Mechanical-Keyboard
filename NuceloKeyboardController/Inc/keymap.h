/*
 * keymap.h
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#ifndef KEYMAP_H_
#define KEYMAP_H_

#include "datatypes.h"
#include "HIDClassCommon.h"

#define KEYBOARD_ROWS 4
#define KEYBOARD_COLS 3

#define KEYMAP( \
    K00, K01, K02, \
    K03, K04, K05, \
    K06, K07, K08, \
         K10 \
) { \
    { K_##K00, K_##K01, K_##K02}, \
    { K_##K03, K_##K04, K_##K05}, \
    { K_##K06, K_##K07, K_##K08}, \
    { K_NO,    K_##K10, K_NO}  \
}

#define KEY(SC)	K_##SC

typedef struct layer_table_entry layer_table_entry_t;
typedef struct keymap_layer keymap_layer_t;

typedef uint8_t key_grid_TypeDef[KEYBOARD_ROWS][KEYBOARD_COLS];

typedef struct{
	const uint8_t grid[KEYBOARD_ROWS][KEYBOARD_COLS];

	uint8_t key;

	char* name;
} layer_init;

struct keymap_layer{
//	key_grid_TypeDef* grid;
	uint8_t grid[KEYBOARD_ROWS][KEYBOARD_COLS];

	uint8_t layer_modifier_key_code;

	char* name;
	uint8_t ID;

	keymap_layer_t* next;
	keymap_layer_t* prev;

};

struct layer_table_entry{
	uint8_t ID;

	uint8_t (*grid)[KEYBOARD_ROWS][KEYBOARD_COLS];

	keymap_layer_t* layer;

	uint8_t key_code;

	layer_table_entry_t* next;
};

typedef struct{
	layer_table_entry_t* layer_head;

	uint8_t entry_count;
} layer_table;

typedef struct keymap_list{
	//LAYERS
	keymap_layer_t* layer_head;

	uint8_t ID_count;

	uint8_t layer_count;

	layer_table* table;

	uint8_t current_layer;
} keymap_list_t;

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
#define K_TAB			HID_KEYBOARD_SC_TAB
#define K_q				HID_KEYBOARD_SC_Q
#define K_w				HID_KEYBOARD_SC_W
#define K_e				HID_KEYBOARD_SC_E
#define K_r				HID_KEYBOARD_SC_R
#define K_t				HID_KEYBOARD_SC_T
#define K_y				HID_KEYBOARD_SC_Y
#define K_u				HID_KEYBOARD_SC_U
#define K_i				HID_KEYBOARD_SC_I
#define K_o				HID_KEYBOARD_SC_O
#define K_p				HID_KEYBOARD_SC_P
#define K_LBRC			HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE
#define K_RBRC			HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE
#define K_ENTER			HID_KEYBOARD_SC_ENTER
#define K_CAPS			HID_KEYBOARD_SC_LOCKING_CAPS_LOCK
#define K_a				HID_KEYBOARD_SC_A
#define K_s				HID_KEYBOARD_SC_S
#define K_d				HID_KEYBOARD_SC_D
#define K_f				HID_KEYBOARD_SC_F
#define K_g				HID_KEYBOARD_SC_G
#define K_h				HID_KEYBOARD_SC_H
#define K_j				HID_KEYBOARD_SC_J
#define K_k				HID_KEYBOARD_SC_K
#define K_l				HID_KEYBOARD_SC_L
#define K_COLON			HID_KEYBOARD_SC_SEMICOLON_AND_COLON
#define K_QUOTE			HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE
#define K_BSLASH		HID_KEYBOARD_SC_BACKSLASH_AND_PIPE
#define K_LSHIFT		HID_KEYBOARD_SC_LEFT_SHIFT
#define K_z				HID_KEYBOARD_SC_Z
#define K_x				HID_KEYBOARD_SC_X
#define K_c				HID_KEYBOARD_SC_C
#define K_v				HID_KEYBOARD_SC_V
#define K_b				HID_KEYBOARD_SC_B
#define K_n				HID_KEYBOARD_SC_N
#define K_m				HID_KEYBOARD_SC_M
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

#define K_LAYER			HID_KEYBOARD_SC_LAYER_FUNCTION
#define K_MACRO_R		HID_KEYBOARD_SC_MACRO_RUN_FUNCTION
#define K_MACRO_S		HID_KEYBOARD_SC_MACRO_SET_FUNCTION
#define K_CLI			HID_KEYBOARD_SC_CLI_FUNCTION
//MEDIA

#define K_PLAY			HID_KEYBOARD_SC_MEDIA_PLAY

//prototypes
keymap_err_t layer_list_init(key_devices_t* keyboard_devices, layer_init* initial_layer_to_add );
keymap_err_t layer_list_append_layer( keymap_list_t* layer_list, layer_init* layer_to_add );
uint8_t layer_list_get_ID( keymap_list_t* layer_list );
keymap_layer_t* layer_list_get_last ( keymap_list_t* layer_list_t );
keymap_layer_t* layer_table_get_layer_w_ID( keymap_list_t* layer_list_t, uint8_t ID );
keymap_layer_t* layer_table_get_current_layer ( keymap_list_t* layer_list_t );
layer_table_entry_t* layer_table_get_last ( keymap_list_t* layer_list_t );
keymap_err_t layer_list_rem_layer_w_ID ( keymap_list_t* layer_list, uint8_t ID );
keymap_err_t layer_table_init ( keymap_list_t* layer_list );
layer_table_entry_t* layer_table_get_second_last (keymap_list_t* layer_list );
keymap_err_t layer_table_append ( keymap_list_t* layer_list, layer_table_entry_t* layer );
keymap_err_t layer_table_rem_last ( keymap_list_t* layer_list );
keymap_err_t layer_table_rem_layer_w_ID ( keymap_list_t* layer_list, uint8_t ID );
uint8_t layer_table_get_ID_w_layer ( keymap_list_t* layer_list, keymap_layer_t* layer );

extern const layer_init keymap_init0;
extern const layer_init keymap_init1;
extern const layer_init keymap_init2;

#endif /* KEYMAP_H_ */
