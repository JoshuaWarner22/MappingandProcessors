//
// Created by Davis Polito on 2/13/25.
//
#include "sysex_chunks.h"
union FloatUnion {
    float f;         // Float value
    uint32_t u;      // View the float as a 32-bit integer
};

void splitUint8To7bit(uint8_t byte, uint8_t chunk[2]) {
    chunk[0] = byte & 0x7F;              // Lower 7 bits
    chunk[1] = (byte >> 7) & 0x01;       // Upper 1 bit
}
 void splitFloatTo7bit (float val, uint8_t chunks[5]) {
     // Split the 32-bit representation into 5 chunks of 7 bits
     FloatUnion floatUnion = {val};

     chunks[0] = (floatUnion.u >> 0) & 0x7F;  // First 7 bits
     chunks[1] = (floatUnion.u >> 7) & 0x7F;  // Next 7 bits
     chunks[2] = (floatUnion.u >> 14) & 0x7F; // Next 7 bits
     chunks[3] = (floatUnion.u >> 21) & 0x7F; // Next 7 bits
     chunks[4] = (floatUnion.u >> 28) & 0xF; // Last 4 bits (padded with 3 zeros for 7 bits
 }
// Unsplit function for uint32_t
uint32_t unsplitUint(const uint8_t chunks[5]) {
    // Combine 5 chunks of 7 bits each into a 32-bit integer
    uint32_t result = 0;
    result |= (chunks[0] << 0);   // First 7 bits
    result |= (chunks[1] << 7);   // Next 7 bits
    result |= (chunks[2] << 14);  // Next 7 bits
    result |= (chunks[3] << 21);  // Next 7 bits
    result |= (chunks[4] << 28);  // Remaining 4 bits (3 bits padding in place)

    return result;
}
// Unsplit function for float (uses the unsplitUint function and a union)
float unsplitFloat(const uint8_t chunks[5]) {
    // Reconstruct the 32-bit integer first
    uint32_t uintVal = unsplitUint(chunks);

    // Use a union to interpret the 32-bit uint as a float
    union {
        uint32_t u;
        float f;
    } floatUnion;

    floatUnion.u = uintVal;
    return floatUnion.f;
}
// Unsplit function for uint8_t
uint8_t unsplitUint8(const uint8_t chunks[2]) {
    // Combine the two 7-bit chunks to reconstruct the original uint8_t value
    uint8_t result = 0;
    result |= (chunks[0] & 0x7F);       // Lower 7 bits
    result |= ((chunks[1] & 0x1) << 7); // Most significant bit

    return result;
}

