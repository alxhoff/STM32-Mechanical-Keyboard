/*
 * lookup.c
 *
 *  Created on: Sep 15, 2017
 *      Author: alxhoff
 */

#include "HIDClassCommon.h"
#include "buffers.h"
#include "config.h"

#define SC(key) HID_KEYBOARD_SC_##key
#define MOD(key) HID_KEYBOARD_MODIFIER_##key

typedef struct scan_code_char {
	const char *unmodified;
	const char *modified;
} scan_code_char_t;

typedef struct scan_code {
	const unsigned char code;
	const unsigned char mod;
} scan_code_t;

scan_code_char_t lookup_char[207] = { [1] = { 0 }, [2] = { 0 }, [3] = { 0 }, [4
		] = { .unmodified = "a", .modified = "A" }, [5] = { .unmodified = "b",
		.modified = "B" }, [6] = { .unmodified = "c", .modified = "C" }, [7] = {
		.unmodified = "d", .modified = "D" }, [8] = { .unmodified = "e",
		.modified = "E" }, [9] = { .unmodified = "f", .modified = "F" }, [10
		] = { .unmodified = "g", .modified = "G" }, [11] = { .unmodified = "h",
		.modified = "H" }, [12] = { .unmodified = "i", .modified = "I" }, [13
		] = { .unmodified = "j", .modified = "J" }, [14] = { .unmodified = "k",
		.modified = "K" }, [15] = { .unmodified = "l", .modified = "L" }, [16
		] = { .unmodified = "m", .modified = "M" }, [17] = { .unmodified = "n",
		.modified = "N" }, [18] = { .unmodified = "o", .modified = "O" }, [19
		] = { .unmodified = "p", .modified = "P" }, [20] = { .unmodified = "q",
		.modified = "Q" }, [21] = { .unmodified = "r", .modified = "R" }, [22
		] = { .unmodified = "s", .modified = "S" }, [23] = { .unmodified = "t",
		.modified = "T" }, [24] = { .unmodified = "u", .modified = "U" }, [25
		] = { .unmodified = "v", .modified = "V" }, [26] = { .unmodified = "W",
		.modified = "W" }, [27] = { .unmodified = "x", .modified = "X" }, [28
		] = { .unmodified = "y", .modified = "Y" }, [29] = { .unmodified = "z",
		.modified = "Z" }, [30] = { .unmodified = "1", .modified = "!" }, [31
		] = { .unmodified = "2", .modified = "@" }, [32] = { .unmodified = "3",
		.modified = "#" }, [33] = { .unmodified = "4", .modified = "$" }, [34
		] = { .unmodified = "5", .modified = "%" }, [35] = { .unmodified = "6",
		.modified = "^" }, [36] = { .unmodified = "7", .modified = "&" }, [37
		] = { .unmodified = "8", .modified = "*" }, [38] = { .unmodified = "9",
		.modified = "(" }, [39] = { .unmodified = "0", .modified = ")" }, [40
		] = { .unmodified = "\n", .modified = "\n" }, [41] = { 0 },
		[42] = { 0 }, [43] = { 0 }, [44] = { .unmodified = "", .modified = "" },
		[45] = { .unmodified = "-", .modified = "_" }, [46] = { .unmodified =
				"=", .modified = "+" }, [47] = { .unmodified = "[", .modified =
				"{" }, [48] = { .unmodified = "]", .modified = "}" }, [49] = {
				.unmodified = "", .modified = "|" },
		//TODO
		[50] = { "b" },

		[51] = { .unmodified = ";", .modified = ":" },
		[52] = { .unmodified = "", // "
				.modified = "" }, //"
		[53] = { .unmodified = "`", .modified = "~" }, [54] = { .unmodified =
				",", .modified = "<" }, [55] = { .unmodified = ".", .modified =
				">" }, [56] = { .unmodified = "/", .modified = "?" }, [57] = {
				.unmodified = "CAP", .modified = "CAP" },
		[58] = { .unmodified = "F1", .modified = "F1" }, //F1
		[59] = { .unmodified = "F2", .modified = "F2" }, [60] = { .unmodified =
				"F3", .modified = "F3" }, [61] = { .unmodified = "F4",
				.modified = "F4" }, [62] = { .unmodified = "F5", .modified =
				"F5" }, [63] = { .unmodified = "F6", .modified = "F6" }, [64
				] = { .unmodified = "F7", .modified = "F7" }, [65] = {
				.unmodified = "F8", .modified = "F8" }, [66] = { .unmodified =
				"F9", .modified = "F9" }, [67] = { .unmodified = "F10",
				.modified = "F10" }, [68] = { .unmodified = "F11", .modified =
				"F11" }, [69] = { .unmodified = "F12", .modified = "F12" }, [70
				] = { .unmodified = "PSC", .modified = "PSC" }, [71] = {
				.unmodified = "SLK", .modified = "SLK" }, [72] = { .unmodified =
				"PAU", .modified = "PAU" }, [73] = { .unmodified = "INS",
				.modified = "INS" }, [74] = { .unmodified = "HOM", .modified =
				"HOM" }, [75] = { .unmodified = "PGU", .modified = "PGU" }, [76
				] = { .unmodified = "DEL", .modified = "DEL" }, [77] = {
				.unmodified = "END", .modified = "END" }, [78] = { .unmodified =
				"PGD", .modified = "PGD" }, [79] = { .unmodified = "RIG",
				.modified = "RIG" }, [80] = { .unmodified = "LEF", .modified =
				"LEF" }, [81] = { .unmodified = "DWN", .modified = "DWN" }, [82
				] = { .unmodified = "UP", .modified = "UP" }, [83] = {
				.unmodified = "NLK", .modified = "NLK" }, [84] = { .unmodified =
				"KSP", .modified = "KSP" }, [85] = { .unmodified = "KAS",
				.modified = "KAS" }, [86] = { .unmodified = "KMI", .modified =
				"KMI" }, [87] = { .unmodified = "KPL", .modified = "KPL" }, [88
				] = { .unmodified = "KEN", .modified = "KEN" }, [89] = {
				.unmodified = "K1", .modified = "END" }, [90] = { .unmodified =
				"K2", .modified = "DWN" }, [91] = { .unmodified = "K3",
				.modified = "PGD" }, [92] = { .unmodified = "K4", .modified =
				"LEF" }, [93] = { .unmodified = "K5", .modified = 0 }, [94] = {
				.unmodified = "K6", .modified = "RIG" }, [95] = { .unmodified =
				"K7", .modified = "HOM" }, [96] = { .unmodified = "K8",
				.modified = "UP" }, [97] = { .unmodified = "K9", .modified =
				"PGU" }, [98] = { .unmodified = "K0", .modified = "INS" }, [99
				] = { .unmodified = "KDO", .modified = "DEL" }, [100] = { "b" },
		[101] = { "b" }, [102] = { "b" }, [103] = { "b" }, [104] = {
				.unmodified = "F13", .modified = "F13" }, [105] = {
				.unmodified = "F14", .modified = "F14" }, [106] = {
				.unmodified = "F15", .modified = "F15" }, [107] = {
				.unmodified = "F16", .modified = "F16" }, [108] = {
				.unmodified = "F17", .modified = "F17" }, [109] = {
				.unmodified = "F18", .modified = "F18" }, [110] = {
				.unmodified = "F19", .modified = "F19" }, [111] = {
				.unmodified = "F20", .modified = "F20" }, [112] = {
				.unmodified = "F21", .modified = "F21" }, [113] = {
				.unmodified = "F22", .modified = "F22" }, [114] = {
				.unmodified = "F23", .modified = "F23" }, [115] = {
				.unmodified = "F24", .modified = "F24" }, [116] = {
				.unmodified = "EXEC", .modified = "EXEC" }, [117] = { "b" },
		[118] = { "b" }, [119] = { "b" }, [120] = { "b" }, [121] = { "b" }, [122
				] = { "b" }, [123] = { "b" }, [124] = { "b" }, [125] = { "b" },
		[126] = { "b" }, [127] = { "b" }, [128] = { "b" }, [129] = { "b" }, [130
				] = { "b" }, [131] = { "b" }, [132] = { "b" }, [133] = { "b" },
		[134] = { "b" }, [135] = { "b" }, [136] = { "b" }, [137] = { "b" }, [138
				] = { "b" }, [139] = { "b" }, [140] = { "b" }, [141] = { "b" },
		[142] = { "b" }, [143] = { "b" }, [144] = { "b" }, [145] = { "b" }, [146
				] = { "b" }, [147] = { "b" }, [148] = { "b" }, [149] = { "b" },
		[150] = { "b" }, [151] = { "b" }, [152] = { "b" }, [153] = { "b" }, [154
				] = { "b" }, [155] = { "b" }, [156] = { "b" }, [157] = { "b" },
		[158] = { "b" }, [159] = { "b" }, [160] = { "b" }, [161] = { "b" }, [162
				] = { "b" }, [163] = { "b" }, [164] = { "b" }, [165] = { "b" },
		[166] = { "b" }, [167] = { "b" }, [168] = { "b" }, [169] = { "b" }, [170
				] = { "b" }, [171] = { "b" }, [172] = { "b" }, [173] = { "b" },
		[174] = { "b" }, [175] = { "b" }, [176] = { "b" }, [177] = { "b" }, [178
				] = { "b" }, [179] = { "b" }, [180] = { "b" }, [181] = { "b" },
		[182] = { "b" }, [183] = { "b" }, [184] = { "b" }, [185] = { "b" }, [186
				] = { "b" }, [187] = { "b" }, [188] = { "b" }, [189] = { "b" },
		[190] = { "b" }, [191] = { "b" }, [192] = { "b" }, [193] = { "b" }, [194
				] = { "b" }, [195] = { "b" }, [196] = { "b" }, [197] = { "b" },
		[198] = { "b" }, [199] = { "b" }, [200] = { "b" }, [201] = { "b" }, [202
				] = { "b" }, [203] = { "b" }, [204] = { "b" }, [205] = { "b" },
		[206] = { "b" } };

scan_code_t lookup_sc[256] = { [32]= { SC(SPACE), 0 }, [33]= { SC(
		1_AND_EXCLAMATION), MOD(RIGHTSHIFT) }, [34]= { SC(APOSTROPHE_AND_QUOTE),
		MOD(RIGHTSHIFT) }, [35]= { SC(3_AND_HASHMARK), MOD(RIGHTSHIFT) }, [36
		]= { SC(4_AND_DOLLAR), MOD(RIGHTSHIFT) }, [37]= { SC(5_AND_PERCENTAGE),
		MOD(RIGHTSHIFT) }, [38]= { SC(7_AND_AMPERSAND), MOD(RIGHTSHIFT) }, [39
		]= { SC(APOSTROPHE_AND_QUOTE), 0 }, [40]= { SC(
		9_AND_OPENING_PARENTHESIS), MOD(RIGHTSHIFT) }, [41]= { SC(
		0_AND_CLOSING_PARENTHESIS), MOD(RIGHTSHIFT) }, [42]= { SC(
		8_AND_ASTERISK), MOD(RIGHTSHIFT) }, [43]= { SC(EQUAL_AND_PLUS), MOD(
		RIGHTSHIFT) }, [44]= { SC(COMMA_AND_LESS_THAN_SIGN), 0 }, [45]= { SC(
		MINUS_AND_UNDERSCORE), 0 }, [46]= { SC(DOT_AND_GREATER_THAN_SIGN), 0 },
		[47]= { SC(SLASH_AND_QUESTION_MARK), 0 }, [48]= { SC(
				0_AND_CLOSING_PARENTHESIS), 0 }, [49]= { SC(1_AND_EXCLAMATION),
				0 }, [50]= { SC(2_AND_AT), 0 }, [51]= { SC(3_AND_HASHMARK), 0 },
		[52]= { SC(4_AND_DOLLAR), 0 }, [53]= { SC(5_AND_PERCENTAGE), 0 }, [54
				]= { SC(6_AND_CARET), 0 }, [55]= { SC(7_AND_AMPERSAND), 0 }, [56
				]= { SC(8_AND_ASTERISK), 0 }, [57]= { SC(
				9_AND_OPENING_PARENTHESIS), 0 }, [58]= { SC(
				SEMICOLON_AND_COLON), MOD(RIGHTSHIFT) }, [59]= { SC(
				SEMICOLON_AND_COLON), 0 }, [60]= { SC(COMMA_AND_LESS_THAN_SIGN),
				MOD(RIGHTSHIFT) }, [61]= { SC(EQUAL_AND_PLUS), 0 }, [62]= { SC(
				DOT_AND_GREATER_THAN_SIGN), MOD(RIGHTSHIFT) }, [63]= { SC(
				SLASH_AND_QUESTION_MARK), MOD(RIGHTSHIFT) }, [65]= { SC(A), MOD(
				RIGHTSHIFT) }, [66]= { SC(B), MOD(RIGHTSHIFT) }, [67]= { SC(C),
				MOD(RIGHTSHIFT) }, [68]= { SC(D), MOD(RIGHTSHIFT) }, [69]= { SC(
				E), MOD(RIGHTSHIFT) }, [70]= { SC(F), MOD(RIGHTSHIFT) }, [71]= {
				SC(G), MOD(RIGHTSHIFT) }, [72]= { SC(H), MOD(RIGHTSHIFT) }, [73
				]= { SC(I), MOD(RIGHTSHIFT) }, [74]= { SC(J), MOD(RIGHTSHIFT) },
		[75]= { SC(K), MOD(RIGHTSHIFT) }, [76]= { SC(L), MOD(RIGHTSHIFT) }, [77
				]= { SC(M), MOD(RIGHTSHIFT) }, [78]= { SC(N), MOD(RIGHTSHIFT) },
		[79]= { SC(O), MOD(RIGHTSHIFT) }, [80]= { SC(P), MOD(RIGHTSHIFT) }, [81
				]= { SC(Q), MOD(RIGHTSHIFT) }, [82]= { SC(R), MOD(RIGHTSHIFT) },
		[83]= { SC(S), MOD(RIGHTSHIFT) }, [84]= { SC(T), MOD(RIGHTSHIFT) }, [85
				]= { SC(U), MOD(RIGHTSHIFT) }, [86]= { SC(V), MOD(RIGHTSHIFT) },
		[87]= { SC(W), MOD(RIGHTSHIFT) }, [88]= { SC(X), MOD(RIGHTSHIFT) }, [89
				]= { SC(Y), MOD(RIGHTSHIFT) }, [90]= { SC(Z), MOD(RIGHTSHIFT) },
		[91]= { SC(OPENING_BRACKET_AND_OPENING_BRACE), MOD(RIGHTSHIFT) }, [92
				]= { SC(BACKSLASH_AND_PIPE), MOD(RIGHTSHIFT) }, [93]= { SC(
				CLOSING_BRACKET_AND_CLOSING_BRACE), MOD(RIGHTSHIFT) }, [94]= {
				SC(6_AND_CARET), MOD(RIGHTSHIFT) }, [95]= { SC(
				MINUS_AND_UNDERSCORE), MOD(RIGHTSHIFT) }, [96]= { SC(
				NON_US_HASHMARK_AND_TILDE), 0 }, [97]= { SC(A), 0 }, [98]= { SC(
				B), 0 }, [99]= { SC(C), 0 }, [100]= { SC(D), 0 }, [101]= { SC(
				E), 0 }, [102]= { SC(F), 0 }, [103]= { SC(G), 0 }, [104]= { SC(
				H), 0 }, [105]= { SC(I), 0 }, [106]= { SC(J), 0 }, [107]= { SC(
				K), 0 }, [108]= { SC(L), 0 }, [109]= { SC(M), 0 }, [110]= { SC(
				N), 0 }, [111]= { SC(O), 0 }, [112]= { SC(P), 0 }, [113]= { SC(
				Q), 0 }, [114]= { SC(R), 0 }, [115]= { SC(S), 0 }, [116]= { SC(
				T), 0 }, [117]= { SC(U), 0 }, [118]= { SC(V), 0 }, [119]= { SC(
				W), 0 }, [120]= { SC(X), 0 }, [121]= { SC(Y), 0 }, [122]= { SC(
				Z), 0 }, [123]= { SC(OPENING_BRACKET_AND_OPENING_BRACE), MOD(
				RIGHTSHIFT) }, [124
				]= { SC(BACKSLASH_AND_PIPE), MOD(RIGHTSHIFT) }, [125]= { SC(
				CLOSING_BRACKET_AND_CLOSING_BRACE), MOD(RIGHTSHIFT) }, [126]= {
				SC(NON_US_HASHMARK_AND_TILDE), MOD(RIGHTSHIFT) }, [232]= { SC(
				MEDIA_PLAY), 0 }, };

const char* lookup_get_char(unsigned char sc, unsigned char mod) {
	if (mod)
		return lookup_char[sc].modified;

	return lookup_char[sc].unmodified;
}

unsigned char lookup_get_key(unsigned char character) {
	return lookup_sc[character].code;
}

unsigned char lookup_get_mod(unsigned char character) {
	return lookup_sc[character].mod;
}

unsigned char lookupCheckStateChange(unsigned char sc) {
	//TODO
	switch (sc) {
	case SC(LAYER_FUNCTION):
		break;
	case SC(MACRO_RUN_FUNCTION):
		break;
	case SC(MACRO_SET_FUNCTION):
		break;
	case SC(CLI_FUNCTION):
		break;
	default:
		break;
	}
	return 0;
}

//look if the key is a key like caps lock
unsigned char lookupCaps(unsigned char sc) {
	switch (sc) {
	case SC(LOCKING_CAPS_LOCK):
		return 1;
	default:
		break;
	}
	return 0;
}

