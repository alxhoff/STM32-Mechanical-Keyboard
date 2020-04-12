#ifndef __MATRIX_SCANNER_H__
#define __MATRIX_SCANNER_H__ 

struct matrix_scanner_ops {
    int8_t (*set_col)(uint8_t col);
    int8_t (*read_row)(uint8_t row);
};

typedef void* matrix_scanner_handle_t;

matrix_scanner_handle_t matrixScannerInit(struct matrix_scanner_ops *ops);
int8_t *matrixScannerScan(matrix_scanner_handle_t ms);

#endif // __MATRIX_SCANNER_H__
