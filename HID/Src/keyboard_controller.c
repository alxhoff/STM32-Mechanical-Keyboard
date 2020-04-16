
#include "keyboard_controller.h"
#include "keymap.h"
#include "scanbuf.h"

/*
 * Store layer modifiers in a bitmask that can be AND'd with input bitmaps
 */ 

struct keyboard_controller {
    uint8_t row_count;
    uint8_t col_count;
    struct keyboard_driver_ops *driver_ops;
};

/*
 * The keyboard controller acts as a HUB between the keyboard driver operations,
 * scanbuf that is returned from polling the keyboard and the buffer resolver.
 */
keyboard_controller_handle_t KeyboardControllerInit(struct keyboard_driver_ops *ops)
{
    if((!ops->set_col) || (!ops->read_row))
        return NULL;

    struct keyboard_controller *ms = (struct keyboard_controller *) calloc(1, sizeof(struct keyboard_controller));
    if(!ms)
        return NULL;

    ms->row_count = getKeyboardRowCount();
    ms->col_count = getKeyboardColCount();
    ms->driver_ops = ops;

    return (keyboard_controller_handle_t) ms;
}

int8_t *KeyboardControllerScan(keyboard_controller_handle_t controller)
{
    struct keyboard_controller *driver = (struct keyboard_controller *) controller;
    static uint8_t row, col;

    for(col = 0; col < driver->col_count; col++){
        (driver->driver_ops->set_col)(col);
        for(row = 0; row < driver->row_count; row++){
            /** scanbufSetBit(keymapGetScanCode(col, row)); */
        }
    }
    scanbufCompileBuf();
    scanbufProcessBuf();
}
