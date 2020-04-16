#ifndef __SCANBUF_H__
#define __SCANBUF_H__ 

#include <stdint.h>
#include <stdlib.h>

/*
 * A scanbuf is a bitmap buffer that is fed bitmap bits and produces a full bitmap
 * buffer as well as a 6KRO buffer and a layer modifier buffer.
 */
typedef void* scanBuf_handle_t;

scanBuf_handle_t scanbufAlloc(void);
void scanBufSetBit(scanBuf_handle_t handle, uint8_t index);
void scanBufClear(scanBuf_handle_t);
void scanbufCompileSixKeyBitmap(scanBuf_handle_t handle);
size_t scanBufGetLength(scanBuf_handle_t handle);
uint8_t *scanBufGetFullBitmap(scanBuf_handle_t handle);
int8_t scanBufGetSixKeyBitmap(scanBuf_handle_t handle, uint8_t *buffer);
int8_t scanBufGetLayerModBitmap(scanBuf_handle_t handle, uint8_t *buffer);

#endif // __SCANBUF_H__
