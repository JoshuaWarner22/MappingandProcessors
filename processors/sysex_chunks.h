//
// Created by Davis Polito on 2/13/25.
//

#ifndef LEAF_MODULES_SYSEX_CHUNKS_H
#define LEAF_MODULES_SYSEX_CHUNKS_H
#ifdef __cplusplus
#include <stdio.h>
#include <stdint.h>
#endif
void splitUint8To7bit(uint8_t byte, uint8_t chunked[2]);
void splitFloatTo7bit(float val,uint8_t chunked[5]);
uint8_t unsplitUint8(const uint8_t chunks[2]);
                     float unsplitFloat(const uint8_t chunks[5]);

#endif //LEAF_MODULES_SYSEX_CHUNKS_H
