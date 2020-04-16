
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "scanbuf.h"
#include "config.h"

#define SCANBUF_LEN (MAX_SUPPORTED_KEY_VAL / 8 + 1)
#define SCANBUF_CAST(handle) ((struct scanBuf *)handle)

const uint8_t zero_bitmap[SCANBUF_LEN] = { 0 };

struct scanBuf {
	uint8_t length;

	uint8_t six_key_bit_count;
	uint8_t six_key_bitmap[SCANBUF_LEN];

	uint8_t keyboard_bitmap[SCANBUF_LEN];

	uint8_t layer_modifier_bitmap[SCANBUF_LEN];
};

static uint8_t checkBit(uint8_t *sb, int8_t scancode)
{
    return (uint8_t) (sb[scancode / 8] >> (scancode % 8) & 0x01);
}

static uint8_t scanbufCheckKeyboardBit(struct scanBuf *sb, int8_t scancode)
{
	return checkBit((uint8_t *)sb->keyboard_bitmap, scancode);
}

static uint8_t sixKeyCheckBit(struct scanBuf *sb, int8_t scancode)
{
	return checkBit((uint8_t *)sb->six_key_bitmap, scancode);
}

static int8_t sixKeyAdd(struct scanBuf *sb, uint8_t scancode)
{
	if (sb->six_key_bit_count < 6) {
		sb->six_key_bitmap[scancode / 8] |= (1 << (scancode % 8));
		sb->six_key_bit_count++;
		return 0;
	}
	return -1;
}

static void decrementByteArray(uint8_t *array, size_t len)
{
	size_t i = 0;

	for (; i < len; i++)
		if (array[i]) {
			array[i]--;
			if (i > 0)
				for (i--; i >= 0; i--)
					array[i] = 0xFF;
			return;
		}
}

static void bytewiseANDArray(uint8_t *arr_1, uint8_t *arr_2, size_t len)
{
	static size_t i;

	for (i = 0; i < len; i++)
		arr_1[i] &= arr_2[i];
}

static void bytewiseXORArray(uint8_t *arr_1, uint8_t *arr_2, size_t len)
{
	static size_t i;

	for (i = 0; i < len; i++)
		arr_1[i] ^= arr_2[i];
}

static uint8_t sixKeyIsEmpty(uint8_t *tmp_six_key_array)
{
	return !memcmp(tmp_six_key_array, zero_bitmap, SCANBUF_LEN);
}

// Return 0 is buffer is not full, otherwise 1
static uint8_t sixKeyCount(struct scanBuf *sb)
{
	uint8_t count = 0;
	static int8_t n[SCANBUF_LEN];
	static int8_t m[SCANBUF_LEN];

	if (sixKeyIsEmpty(sb->six_key_bitmap))
		return 0;

	memcpy(&n, sb->six_key_bitmap, SCANBUF_LEN);

	do {
		count++;
		memcmp((uint8_t *)&m, (uint8_t *)&n, SCANBUF_LEN);
		decrementByteArray((uint8_t *)&m, SCANBUF_LEN);
		bytewiseANDArray((uint8_t *)&n, (uint8_t *)&m, SCANBUF_LEN);
	} while (!sixKeyIsEmpty((uint8_t *)&n));

	return count;
}

static void sixKeyFill(struct scanBuf *sb)
{
	static size_t i;
	static uint8_t j;

	for (i = 0; i < SCANBUF_LEN; i++)
		for (j = 0; j < 8; j++)
			if (scanbufCheckKeyboardBit(sb, i * 8 + j))
				if (sixKeyAdd(sb, i * 8 + j))
					return;
}

/*
 * This function should be called after a scan has finished, it will update
 * the 6KRO buffer and swap the active bitmap with the background bitmap
 */
void scanbufCompileSixKeyBitmap(scanBuf_handle_t handle)
{
    struct scanBuf *sb = SCANBUF_CAST(handle);

	bytewiseXORArray(sb->six_key_bitmap, sb->keyboard_bitmap, sb->length);
	sb->six_key_bit_count = sixKeyCount(sb);
}

scanBuf_handle_t scanbufAlloc(void)
{
	struct scanBuf *ret =
		(struct scanBuf *)calloc(1, sizeof(struct scanBuf));

	if (!ret)
		return NULL;

	ret->length = SCANBUF_LEN;

	return (scanBuf_handle_t)ret;
}

void scanbufSetBit(scanBuf_handle_t handle, uint8_t scancode)
{
	if (scancode < MAX_SUPPORTED_KEY_VAL)
		SCANBUF_CAST(handle)->keyboard_bitmap[scancode / 8] |= (1 << (scancode % 8));
}

void scanbufClear(scanBuf_handle_t handle)
{
	memset(SCANBUF_CAST(handle)->keyboard_bitmap, 0, SCANBUF_LEN);
}

size_t scanbufGetLen(scanBuf_handle_t handle)
{
	return (size_t)SCANBUF_CAST(handle)->length;
}

uint8_t *scanBufGetFullBitmap(scanBuf_handle_t handle)
{
	uint8_t *ret = (uint8_t *)malloc(SCANBUF_CAST(handle)->length);
	if (!ret)
		return NULL;

	memcpy(ret, SCANBUF_CAST(handle)->keyboard_bitmap, SCANBUF_LEN);

	return ret;
}

static uint8_t bitmapToArray(uint8_t *bitmap, uint8_t **buffer, uint8_t length,
			      uint8_t bits_to_find)
{
	uint8_t j, i = 0, found_bits = 0;
    uint8_t required_bits;

    if(bits_to_find)
        required_bits = bits_to_find;
    else {
        required_bits = length;
    }

	uint8_t *ret_buf =
		(uint8_t *)malloc(required_bits);
	if (!ret_buf)
		return -1;

    *buffer = ret_buf;

	for (; i < length; i++) {
		for (j = 0; j < 8; j++) {
			if (found_bits == required_bits)
				goto all_found;

			if ((bitmap[i] >> j) &
			    0x01)
				ret_buf[found_bits++] = i * 8 + j + 1;
		}
	}

all_found:
	return found_bits;
}

/*
 * Converts the stored 6 key bitmap into an array of scan codes.
 * Returns the number of scan codes, ie. the length of the array.
 */
int8_t scanBufGetSixKeyBitmap(scanBuf_handle_t handle, uint8_t *buffer)
{
    return bitmapToArray(SCANBUF_CAST(handle)->six_key_bitmap, &buffer,
            SCANBUF_CAST(handle)->length, SCANBUF_CAST(handle)->six_key_bit_count);
}

int8_t scanBufGetLayerMod(scanBuf_handle_t handle, uint8_t *buffer)
{
    return bitmapToArray(SCANBUF_CAST(handle)->layer_modifier_bitmap, &buffer, 
            SCANBUF_CAST(handle)->length, 0);    
}
