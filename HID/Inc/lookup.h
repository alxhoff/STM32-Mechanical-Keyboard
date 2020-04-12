#ifndef __LOOKUP_H__
#define __LOOKUP_H__

#define SC(key) HID_KEYBOARD_SC_##key
#define MOD(key)

uint8_t lookupModifierFromASCII(unsigned char ASCII);
uint8_t lookupScanCodeFromASCII(unsigned char ASCII);
const char *lookupASCIIFromScanCode(uint8_t scancode, uint8_t modifier);

#endif // __LOOKUP_H__
