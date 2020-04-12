#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__ 

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

#endif // __DESCRIPTORS_H__
