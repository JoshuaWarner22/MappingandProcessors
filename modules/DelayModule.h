//
// Created by Myra Norton on 1/22/26.
//

#ifndef ELECTORSYNTH_DELAYMODULE_H
#define ELECTORSYNTH_DELAYMODULE_H


#include "leaf.h"
#include "defs.h"
#include "leaf-mempool.h"
#include "leaf-delay.h"

typedef enum {
    DelayEventWatchFlag,
    DelayTime,
    DelayGain,
    // DelayFeedback,
    // DelayMix,
    DelayType,
    DelayNumParams
} DelParams;

typedef enum {
    DelayTypeDelay,
    DelayTypeLinear,
    DelayTypeHermite,
    DelayTypeLagrange,
    DelayTypeAllpass,
    DelayTypeTape,
    DelayNumTypes
} DelayTypes;

//the actual frequency setter function
typedef void (*tDelayInternalParamSetFunc)(void*, float);

typedef struct _tDelayModule {
    ModuleHeader header;
    void* theDelay;

    float* dbTableAddress;
    uint32_t dbTableScalar;
    float dbTableOffset;
    float dbTableSizeMinusOne;

    uint32_t delayType;

    float amp;
    float sr;
    float invSr;
    tMempool* mempool;
    tLookupTable* table;
} _tDelayModule;

typedef _tDelayModule* tDelayModule;

//init module
void tDelayModule_init(void** const delay, float* const params, float id, LEAF* const leaf);
void tDelayModule_initToPool(void** const delay, float* const params, float id, tMempool** const mempool, tLookupTable* resTable);

void tDelayModule_free(void** const delay);

// tick
void tDelayModule_tick (tDelayModule const delay, float*);

void tDelayModule_setParameter(tDelayModule const delay, DelParams param_type,float input);

// Non-modulatable setters
void tDelayModule_setDBtoATableLocation (tDelayModule const delay, float* tableAddress, uint32_t tableSize);
void tDelayModule_setSampleRate (tDelayModule const delay, float sr);

#endif // ELECTORSYNTH_DELAYMODULE_H
