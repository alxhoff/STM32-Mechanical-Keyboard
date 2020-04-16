#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__ 

#include <stdint.h>

struct HID_descr_boot_keyboard {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
};

struct HID_descr_keyboard {
    uint8_t id;
    uint8_t modifiers;
    uint8_t keys[6];
};

struct HID_descr_boot_mouse {
    uint8_t id;
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;
};

struct HID_descr_mouse {
    uint8_t buttons;
    int8_t x;
    int8_t y;
};

struct HID_descr_composite {
    uint8_t modifiers;
    uint8_t keys[6];
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;
};

union HID_descr_boot {
    struct HID_descr_boot_keyboard keyboard;
    struct HID_descr_boot_mouse mouse;
};

union HID_descr_IDs {
    struct HID_descr_keyboard keyboard;
    struct HID_descr_mouse mouse;
};

// Available: BOOT_DESCRIPTOR, ID_DESCRIPTORS, COMPOSITE, NKRO
struct HID_report {
    union HID_descr_boot boot;
    union HID_descr_IDs ids;
    struct HID_descr_composite composite;
};

#endif // __DESCRIPTORS_H__
