
/*
 * Scan buf contains the buffer that is used for storing values scanned from the 
 * keyboard matrix before they are processed for transmission.
 *
 * 6 Key scanbuffer
 *
 * The 6 key buffer is used to keep track of 6 keys needed bot BOOT keyboards that
 * only support 6KRO. After the keyboard matrix is scanned the 6 key bitmap from the
 * previous scan is bitwise &'d with the newly scanned bitmap, this gives common keys.
 * The number of common keys is then counted using Brian Kernighan's algorithm.
 * The 6 key buffer is then filled from 0x04, stepping up bitwise. Changing this
 * handling of filling the 6 key buffer is a TODO.
 *
 */

#include <stdlib.h>

#define SCANBUF_LEN (MAX_SUPPORTED_KEY_VAL / 8 + 1)

// First bitmap is used for storing a bitmap of entire keyboard. 
// The second buffer keeps track of the six keys for 6KRO reports. 
// The third is an empty buffer used as a comparison to check if a buffer is empty.

#define FULL_BITMAP scanbuf_bitmap[0]
#define SIXKRO_BITMAP scanbuf_bitmap[1]
#define SIXKRO_EMPTY_BITMAP scanbuf_bitmap[2]

uin8_t scanbuf6_count = 0;

uint8_t scanbuf_bitmap[3][SCANBUF_LEN] = {0};
uint8_t cur_scanbuf = 0;

#ifdef SUPPORT_16BIT_SCANCODES
int8_t scanbufSetBit(uint16_t scancode)
#else 
int8_t scanbufSetBit(uint8_t scancode)
#endif // SUPPORT_16BIT_SCANCODES
{
    if(scancode > MAX_SUPPORTED_KEY_VAL)
        return -1;

    FULL_BITMAP[scancode / 8] |= (1 << (scancode % 8));
}

#ifdef SUPPORT_16BIT_SCANCODES
static uint8_t checkBit(uint8_t *sb, uint16_t scancode)
#else 
static uint8_t checkBit(uint8_t *sb, int8_t scancode)
#endif // SUPPORT_16BIT_SCANCODES
{
    return (uint8_t) (sb[scancode / 8] >> (scancode % 8) & 0x01;
}

#ifdef SUPPORT_16BIT_SCANCODES
uint8_t scanbufCheckBit(uint16_t scancode)
#else 
uint8_t scanbufCheckBit(int8_t scancode)
#endif // SUPPORT_16BIT_SCANCODES
{
    return checkBit(FULL_BITMAP, scancode);
}

#ifdef SUPPORT_16BIT_SCANCODES
static uint8_t sixKeyCheckBit(uint16_t scancode)
#else 
static uint8_t sixKeyCheckBit(int8_t scancode)
#endif // SUPPORT_16BIT_SCANCODES
{
    return checkBit(SIXKRO_BITMAP, scancode);
}


#ifdef SUPPORT_16BIT_SCANCODES
static int8_t sixKeyAdd(uint16_t scancode)
#else 
static int8_t sixKeyAdd(uint8_t scancode)
#endif // SUPPORT_16BIT_SCANCODES
{
    if(scanbuf6_count < 6){
        SIXKRO_BITMAP[scancode/8] |= (1 << (scancode % 8));
        scanbuf6_count++;
        return 0;
    }
    return -1;
}

static void decrementByteArray(uint8_t *array, size_t len)
{
    size_t i = 0;

    for(; i < len; i++)
        if(array[i]){
            array[i]--;
            if(i > 0) 
                for(i-- ; i >= 0 ; i--)
                    array[i] = 0xFF;
            return;
        }
}

static void bytewiseANDArray(uint8_t *arr_1, uint8_t *arr_2, size_t len)
{
    static size_t i;

    for(i = 0; i < len; i++)
        arr_1[i] &= arr_2[i];
}

static void bytewiseXORArray(uint8_t *arr_1, uint8_t *arr_2, size_t len)
{
    static size_t i;

    for(i = 0; i < len; i++)
        arr_1[i] ^= arr_2[i];
}

static uint8_t sixKeyIsEmpty(uint8_t *tmp_6_array)
{
    return !memcmp(tmp_6_array, SIXKRO_EMPTY_BITMAP, SCANBUF_LEN);
}

// Return 0 is buffer is not full, otherwise 1
uint8_t sixKeyCount(void) 
{
    uint8_t count = 0;
    static int8_t n[SCANBUF_LEN];
    static int8_t m[SCANBUF_LEN];

    if(sixKeyIsEmpty(SIXKRO_BITMAP))
        return 0;

    memcpy(&n, SIXKRO_BITMAP, SCANBUF_LEN);

    do{
        count++;
        memcmp(&m, &n, SCANBUF_LEN);
        decrementByteArray(&m, SCANBUF_LEN);
        bytewiseANDArray(&n, &m, SCANBUF_LEN);
    }while(!sixKeyIsEmpty(&n));

    return count;
}

void scanbufClear(void)
{
    memset(FULL_BITMAP, 0, SCANBUF_LEN);
}

static void sixKeyFill(void)
{
    static size_t i;
    static uint8_t j;

    for(i = 0; i < SCANBUF_LEN; i++)
        for(j = 0; j < 8; j++)
            if(scanbufCheckBit(i * 8 + j))
                if(sixKeyAdd(i * 8 + j))
                    return;
}

/*
 * This function should be called after a scan has finished, it will update
 * the 6KRO buffer and swap the active bitmap with the background bitmap
 */
void scanbufSwapBuf(void)
{
    static uint8_t count;

    bytewiseXORArray(SIXKRO_BITMAP, FULL_BITMAP);
    scanbuf6_count = sixKeyCount();

}

size_t scanbufGetLen(void)
{
    return (size_t) SCANBUF_LEN;
}

