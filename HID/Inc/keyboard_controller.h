#ifndef __keyboard_controller_H__
#define __keyboard_controller_H__ 

#include <stdint.h>

struct keyboard_driver_ops {
    int8_t (*set_col)(uint8_t col);
    void (*read_row)(uint8_t row);
};

typedef void* keyboard_controller_handle_t;

keyboard_controller_handle_t KeyboardControllerInit(struct keyboard_driver_ops *ops);
int8_t *KeyboardControllerScan(keyboard_controller_handle_t ms);

#endif // __keyboard_controller_H__
