/*
 * keymap.h
 *
 *  Created on: Aug 16, 2017
 *      Author: alxhoff
 */

#ifndef KEYMAP_H_
#define KEYMAP_H_

#include "config.h"
#include "ll.h"

#define K_LAYER			HID_KEYBOARD_SC_LAYER_FUNCTION
#define K_MACRO_R		HID_KEYBOARD_SC_MACRO_RUN_FUNCTION
#define K_MACRO_S		HID_KEYBOARD_SC_MACRO_SET_FUNCTION
#define K_CLI			HID_KEYBOARD_SC_CLI_FUNCTION

#define KM(KEY) HID_KEYBOARD_SC_##KEY
#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12, K13, K14, 	\
    K15, K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, 	\
    K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K40, K41, K42, K43, K44, 	\
    K45, K46, K47, K48, K49, K50, K51, K52, K53, K54, K55, K56, K57, K58, K59, 	\
	K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K70, K71, K72, K73, K74	\
) { \
    { KM(K00), KM(K01), KM(K02), KM(K03), KM(K04), KM(K05), KM(K06), KM(K07), \
		KM(K08), KM(K09), KM(K10), KM(K11), KM(K12), KM(K13), KM(K14)}, \
    { KM(K15), KM(K16), KM(K17), KM(K18), KM(K19), KM(K20), KM(K21), KM(K22), \
		KM(K23), KM(K24), KM(K25), KM(K26), KM(K27), KM(K28), KM(K29)}, \
    { KM(K30), KM(K31), KM(K32), KM(K33), KM(K34), KM(K35), KM(K36), KM(K37), \
			KM(K38), KM(K39), KM(K40), KM(K41), KM(K42), KM(K43), KM(K44)}, \
    { KM(K45), KM(K46), KM(K47), KM(K48), KM(K49), KM(K50), KM(K51), KM(K52), \
			KM(K53), KM(K54), KM(K55), KM(K56), KM(K57), KM(K58), KM(K59)}, \
    { KM(K60), KM(K61), KM(K62), KM(K63), KM(K64), KM(K65), KM(K66), KM(K67), \
			KM(K68), KM(K69), KM(K70), KM(K71), KM(K72), KM(K73), KM(K74)}  \
}

typedef struct keymap_layer keymap_layer_t;

struct keymap_layer {
	struct list_item list;

	unsigned char grid[KEYBOARD_ROWS][KEYBOARD_COLS];
	unsigned char mod_key_code;
	char *name;
	volatile unsigned char ID;
};

//prototypes
void keymapInit(void);
void keymapSetKeyCode(unsigned char row, unsigned char col, unsigned char val);
unsigned char keymapGetKeyCode(unsigned char row, unsigned char col);
void keymapSetCurrentName(char *name);
char* keymapGetCurrentName(void);
void keymapSetCurrentModKeyCode(unsigned char code);
unsigned char keymapGetCurrentModKeyCode(void);
unsigned char keymapGetCurrentID(void);
unsigned char keymapGetLayerCount(void);

#endif /* KEYMAP_H_ */
