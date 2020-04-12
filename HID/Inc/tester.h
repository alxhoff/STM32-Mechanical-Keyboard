#ifndef __TESTER_H__
#define __TESTER_H__ 

#include <stdlib.h>

#include "matrix_scanner.h"

// Probability that row is pressed
#define TESTER_PROB 5
#define PROB_MOD (100 / TESTER_PROB)

#define TESTER_MIN_ROW  1
#define TESTER_MAX_ROW  3
#define TESTER_MIN_COL  3
#define TESTER_MAX_COL  8

uint8_t cur_col;

/*
 * Acts as a dummy function for the matrix scanner ops, returning a random
 * "on" or "off signal"
 */
int8_t testerReadRow(uint8_t row)
{
    if((TESTER_MIN_ROW <= row) && (row <= TESTER_MAX_ROW))
        if((TESTER_MIN_COL <= cur_col) && (cur_col <= TESTER_MAX_COL))
            if(!(ramd() % PROB_MOD))
                return 1;
    return 0;
}

void testerSetCol(uint8_t col)
{
    cur_col = col;
}

struct matrix_scanner_ops tester_ops = { 
    .set_col = testerSetCol,
    .read_row = testerReadRow 
};

#endif // __TESTER_H__
