#include <stdint.h>
#include <stdlib.h>

#include "HIDClassCommon.h"
#include "lookup.h"

#define MAX_SCANCODE HID_KEYBOARD_SC_MEDIA_CALCULATOR

struct ascii_scan_code {
	const char *unmodified;
	const char *modified;
};

struct scan_code_ascii {
	const uint8_t scancode;
	const uint8_t modifier;
};

struct ascii_scan_code ascii_from_sc[HID_KEYBOARD_SC_MEDIA_CALCULATOR] = {
	[SC(A)] = { .unmodified = "a", .modified = "A" },
	[SC(B)] = { .unmodified = "b", .modified = "B" },
	[SC(C)] = { .unmodified = "c", .modified = "C" },
	[SC(D)] = { .unmodified = "d", .modified = "D" },
	[SC(E)] = { .unmodified = "e", .modified = "E" },
	[SC(F)] = { .unmodified = "f", .modified = "F" },
	[SC(G)] = { .unmodified = "g", .modified = "G" },
	[SC(H)] = { .unmodified = "h", .modified = "H" },
	[SC(I)] = { .unmodified = "i", .modified = "I" },
	[SC(J)] = { .unmodified = "j", .modified = "J" },
	[SC(K)] = { .unmodified = "k", .modified = "K" },
	[SC(L)] = { .unmodified = "l", .modified = "L" },
	[SC(M)] = { .unmodified = "m", .modified = "M" },
	[SC(N)] = { .unmodified = "n", .modified = "N" },
	[SC(O)] = { .unmodified = "o", .modified = "O" },
	[SC(P)] = { .unmodified = "p", .modified = "P" },
	[SC(Q)] = { .unmodified = "q", .modified = "Q" },
	[SC(R)] = { .unmodified = "r", .modified = "R" },
	[SC(S)] = { .unmodified = "s", .modified = "S" },
	[SC(T)] = { .unmodified = "t", .modified = "T" },
	[SC(U)] = { .unmodified = "u", .modified = "U" },
	[SC(V)] = { .unmodified = "v", .modified = "V" },
	[SC(W)] = { .unmodified = "W", .modified = "W" },
	[SC(X)] = { .unmodified = "x", .modified = "X" },
	[SC(Y)] = { .unmodified = "y", .modified = "Y" },
	[SC(Z)] = { .unmodified = "z", .modified = "Z" },
	[SC(1_AND_EXCLAMATION)] = { .unmodified = "1", .modified = "!" },
	[SC(2_AND_AT)] = { .unmodified = "2", .modified = "@" },
	[SC(3_AND_HASHMARK)] = { .unmodified = "3", .modified = "#" },
	[SC(4_AND_DOLLAR)] = { .unmodified = "4", .modified = "$" },
	[SC(5_AND_PERCENTAGE)] = { .unmodified = "5", .modified = "%" },
	[SC(6_AND_CARET)] = { .unmodified = "6", .modified = "^" },
	[SC(7_AND_AMPERSAND)] = { .unmodified = "7", .modified = "&" },
	[SC(8_AND_ASTERISK)] = { .unmodified = "8", .modified = "*" },
	[SC(9_AND_OPENING_PARENTHESIS)] = { .unmodified = "9", .modified = "(" },
	[SC(0_AND_CLOSING_PARENTHESIS)] = { .unmodified = "0", .modified = ")" },
	[SC(ENTER)] = { .unmodified = "\n", .modified = "\n" },
	[SC(MINUS_AND_UNDERSCORE)] = { .unmodified = "-", .modified = "_" },
	[SC(EQUAL_AND_PLUS)] = { .unmodified = "=", .modified = "+" },
	[SC(OPENING_BRACKET_AND_OPENING_BRACE)] = { .unmodified = "[",
						    .modified = "{" },
	[SC(CLOSING_BRACKET_AND_CLOSING_BRACE)] = { .unmodified = "]",
						    .modified = "}" },
	[SC(BACKSLASH_AND_PIPE)] = { .unmodified = "", .modified = "|" },
	[SC(NON_US_HASHMARK_AND_TILDE)] = { .unmodified = "#",
					    .modified = "~" },
	[SC(SEMICOLON_AND_COLON)] = { .unmodified = ";", .modified = ":" },
	[SC(APOSTROPHE_AND_QUOTE)] = { .unmodified = "'", .modified = "\"" },
	[SC(GRAVE_ACCENT_AND_TILDE)] = { .unmodified = "`", .modified = "~" },
	[SC(COMMA_AND_LESS_THAN_SIGN)] = { .unmodified = ",", .modified = "<" },
	[SC(DOT_AND_GREATER_THAN_SIGN)] = { .unmodified = ".",
					    .modified = ">" },
	[SC(SLASH_AND_QUESTION_MARK)] = { .unmodified = "/", .modified = "?" },
	[SC(CAPS_LOCK)] = { .unmodified = "CAP", .modified = "CAP" },
	[SC(F1)] = { .unmodified = "F1", .modified = "F1" },
	[SC(F2)] = { .unmodified = "F2", .modified = "F2" },
	[SC(F3)] = { .unmodified = "F3", .modified = "F3" },
	[SC(F4)] = { .unmodified = "F4", .modified = "F4" },
	[SC(F5)] = { .unmodified = "F5", .modified = "F5" },
	[SC(F6)] = { .unmodified = "F6", .modified = "F6" },
	[SC(F7)] = { .unmodified = "F7", .modified = "F7" },
	[SC(F8)] = { .unmodified = "F8", .modified = "F8" },
	[SC(F9)] = { .unmodified = "F9", .modified = "F9" },
	[SC(F10)] = { .unmodified = "F10", .modified = "F10" },
	[SC(F11)] = { .unmodified = "F11", .modified = "F11" },
	[SC(F12)] = { .unmodified = "F12", .modified = "F12" },
	[SC(PRINT_SCREEN)] = { .unmodified = "PSC", .modified = "PSC" },
	[SC(SCROLL_LOCK)] = { .unmodified = "SLK", .modified = "SLK" },
	[SC(PAUSE)] = { .unmodified = "PAU", .modified = "PAU" },
	[SC(INSERT)] = { .unmodified = "INS", .modified = "INS" },
	[SC(HOME)] = { .unmodified = "HOM", .modified = "HOM" },
	[SC(PAGE_UP)] = { .unmodified = "PGU", .modified = "PGU" },
	[SC(DELETE)] = { .unmodified = "DEL", .modified = "DEL" },
	[SC(END)] = { .unmodified = "END", .modified = "END" },
	[SC(PAGE_DOWN)] = { .unmodified = "PGD", .modified = "PGD" },
	[SC(RIGHT_ARROW)] = { .unmodified = "RIG", .modified = "RIG" },
	[SC(LEFT_ARROW)] = { .unmodified = "LEF", .modified = "LEF" },
	[SC(DOWN_ARROW)] = { .unmodified = "DWN", .modified = "DWN" },
	[SC(UP_ARROW)] = { .unmodified = "UP", .modified = "UP" },
	[SC(NUM_LOCK)] = { .unmodified = "NLK", .modified = "NLK" },
};

struct scan_code_ascii sc_from_ascii[HID_KEYBOARD_SC_MEDIA_CALCULATOR] = {
	[' '] = { SC(SPACE), 0 },
	['!'] = { SC(1_AND_EXCLAMATION), MOD(RIGHTSHIFT) },
	['\''] = { SC(2_AND_AT), MOD(RIGHTSHIFT) },
	['3'] = { SC(3_AND_HASHMARK), MOD(RIGHTSHIFT) },
	['4'] = { SC(4_AND_DOLLAR), MOD(RIGHTSHIFT) },
	['%'] = { SC(5_AND_PERCENTAGE), MOD(RIGHTSHIFT) },
	['^'] = { SC(6_AND_CARET), MOD(RIGHTSHIFT) },
	['&'] = { SC(7_AND_AMPERSAND), MOD(RIGHTSHIFT) },
	['*'] = { SC(8_AND_ASTERISK), 0 },
	['('] = { SC(9_AND_OPENING_PARENTHESIS), MOD(RIGHTSHIFT) },
	[')'] = { SC(0_AND_CLOSING_PARENTHESIS), MOD(RIGHTSHIFT) },
	['+'] = { SC(EQUAL_AND_PLUS), MOD(RIGHTSHIFT) },
	[','] = { SC(COMMA_AND_LESS_THAN_SIGN), 0 },
	['_'] = { SC(MINUS_AND_UNDERSCORE), MOD(RIGHTSHIFT) },
	['-'] = { SC(MINUS_AND_UNDERSCORE), 0 },
	['.'] = { SC(DOT_AND_GREATER_THAN_SIGN), 0 },
	['/'] = { SC(SLASH_AND_QUESTION_MARK), 0 },
	['0'] = { SC(0_AND_CLOSING_PARENTHESIS), 0 },
	['1'] = { SC(1_AND_EXCLAMATION), 0 },
	['2'] = { SC(2_AND_AT), 0 },
	['3'] = { SC(3_AND_HASHMARK), 0 },
	['4'] = { SC(4_AND_DOLLAR), 0 },
	['5'] = { SC(5_AND_PERCENTAGE), 0 },
	['6'] = { SC(6_AND_CARET), 0 },
	['7'] = { SC(7_AND_AMPERSAND), 0 },
	['8'] = { SC(8_AND_ASTERISK), 0 },
	['9'] = { SC(9_AND_OPENING_PARENTHESIS), 0 },
	[':'] = { SC(SEMICOLON_AND_COLON), MOD(RIGHTSHIFT) },
	[';'] = { SC(SEMICOLON_AND_COLON), 0 },
	['<'] = { SC(COMMA_AND_LESS_THAN_SIGN), MOD(RIGHTSHIFT) },
	['='] = { SC(EQUAL_AND_PLUS), 0 },
	['>'] = { SC(DOT_AND_GREATER_THAN_SIGN), MOD(RIGHTSHIFT) },
	['?'] = { SC(SLASH_AND_QUESTION_MARK), MOD(RIGHTSHIFT) },
	['A'] = { SC(A), MOD(RIGHTSHIFT) },
	['B'] = { SC(B), MOD(RIGHTSHIFT) },
	['C'] = { SC(C), MOD(RIGHTSHIFT) },
	['D'] = { SC(D), MOD(RIGHTSHIFT) },
	['E'] = { SC(E), MOD(RIGHTSHIFT) },
	['F'] = { SC(F), MOD(RIGHTSHIFT) },
	['G'] = { SC(G), MOD(RIGHTSHIFT) },
	['H'] = { SC(H), MOD(RIGHTSHIFT) },
	['I'] = { SC(I), MOD(RIGHTSHIFT) },
	['J'] = { SC(J), MOD(RIGHTSHIFT) },
	['K'] = { SC(K), MOD(RIGHTSHIFT) },
	['L'] = { SC(L), MOD(RIGHTSHIFT) },
	['M'] = { SC(M), MOD(RIGHTSHIFT) },
	['N'] = { SC(N), MOD(RIGHTSHIFT) },
	['O'] = { SC(O), MOD(RIGHTSHIFT) },
	['P'] = { SC(P), MOD(RIGHTSHIFT) },
	['Q'] = { SC(Q), MOD(RIGHTSHIFT) },
	['R'] = { SC(R), MOD(RIGHTSHIFT) },
	['S'] = { SC(S), MOD(RIGHTSHIFT) },
	['T'] = { SC(T), MOD(RIGHTSHIFT) },
	['U'] = { SC(U), MOD(RIGHTSHIFT) },
	['V'] = { SC(V), MOD(RIGHTSHIFT) },
	['W'] = { SC(W), MOD(RIGHTSHIFT) },
	['X'] = { SC(X), MOD(RIGHTSHIFT) },
	['Y'] = { SC(Y), MOD(RIGHTSHIFT) },
	['Z'] = { SC(Z), MOD(RIGHTSHIFT) },
	['a'] = { SC(A), 0 },
	['b'] = { SC(B), 0 },
	['b'] = { SC(C), 0 },
	['d'] = { SC(D), 0 },
	['e'] = { SC(E), 0 },
	['f'] = { SC(F), 0 },
	['g'] = { SC(G), 0 },
	['h'] = { SC(H), 0 },
	['i'] = { SC(I), 0 },
	['j'] = { SC(J), 0 },
	['k'] = { SC(K), 0 },
	['l'] = { SC(L), 0 },
	['m'] = { SC(M), 0 },
	['n'] = { SC(N), 0 },
	['o'] = { SC(O), 0 },
	['p'] = { SC(P), 0 },
	['q'] = { SC(Q), 0 },
	['r'] = { SC(R), 0 },
	['s'] = { SC(S), 0 },
	['t'] = { SC(T), 0 },
	['u'] = { SC(U), 0 },
	['v'] = { SC(V), 0 },
	['w'] = { SC(W), 0 },
	['x'] = { SC(X), 0 },
	['y'] = { SC(Y), 0 },
	['z'] = { SC(Z), 0 },
	['{'] = { SC(OPENING_BRACKET_AND_OPENING_BRACE), MOD(RIGHTSHIFT) },
	['|'] = { SC(BACKSLASH_AND_PIPE), MOD(RIGHTSHIFT) },
	['}'] = { SC(CLOSING_BRACKET_AND_CLOSING_BRACE), MOD(RIGHTSHIFT) },
};

const char *lookupASCIIFromScanCode(uint8_t scancode, uint8_t modifier)
{
    if(scancode > MAX_SCANCODE)
        return NULL;

    if(modifier)
        return ascii_from_sc[scancode].modified;

    return ascii_from_sc[scancode].unmodified;

}

uint8_t lookupScanCodeFromASCII(unsigned char ASCII)
{
    if(ASCII < MAX_SCANCODE)
        return 0x00;

    return sc_from_ascii[ASCII].scancode;
}

uint8_t lookupModifierFromASCII(unsigned char ASCII)
{
    if(ASCII < MAX_SCANCODE)
        return 0x00;

    return sc_from_ascii[ASCII].modifier;
}
