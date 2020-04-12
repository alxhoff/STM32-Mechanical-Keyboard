
#include "matrix_scanner.h"
#include "keymap.h"
#include "scanbuf.h"

/*
 * Store layer modifiers in a bitmask that can be AND'd with input bitmaps
 */ 

struct matrix_scanner {
    uint8_t row_count;
    uint8_t col_count;
    struct matrix_scanner_ops *ops;
};

matrix_scanner_handle_t matrixScannerInit(struct matrix_scanner_ops *ms)
{
    if((!ms->set_col) || (!ms->read_row))
        return NULL;

    struct matrix_scanner ms = (struct matrix_scanner *) calloc(1, sizeof(struct matrix_scanner));
    if(!ms)
        return NULL;

    ms->row_count = getKeyboardRowCount();
    ms->col_count = getKeyboardColCount();
    ms->ops = ops;

    return (matrix_scanner_handle_t) ms;
}

int8_t *matrixScannerScan(matrix_scanner_handle_t ms)
{
    struct matrix_scanner *scanner = (struct matrix_scanner *) ms;
    static uint8_t row, col;

    for(col = 0; col < scanner->col_count; col++){
        (scanner->ops->set_col)(col);
        for(row = 0; row < scanner->row_count; row++){
            scanbufSetBit(keymapGetScanCode(col, row));
        }
    }
    scanbufCompileBuf();
    scanbufProcessBuf();
}
