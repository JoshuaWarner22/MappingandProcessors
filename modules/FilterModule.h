//
// Created by Joshua Warner on 6/13/24.
//

#ifndef SIMPLEOSCMODULE_H
#define SIMPLEOSCMODULE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../LEAF/leaf/leaf.h"
#include "../processors/defs.h"
#include "../processors/processor.h"
#include "../LEAF/leaf/Inc/leaf-mempool.h"
#include "../LEAF/leaf/Inc/leaf-filters.h"


typedef enum {
    FiltMidiPitch,
    FiltCutoff,
    FiltGain,
    FiltResonance,
    FiltKeyfollow,
    FiltType,
    FiltAudioInput,
    FiltNumParams
} FiltParams;

typedef enum {
    FiltTypeLowpass,
    FiltTypeHighpass,
    FiltTypeBandpass,
    FiltTypeDiodeLowpass,
    FiltTypePeak,
    FiltTypeHighShelf,
    FiltTypeLowShelf,
    FiltTypeNotch,
    FiltTypeLadderLowpass,
    FiltNumTypes
} FiltTypes;

//the actual frequency setter function
typedef void (*tFiltInternalParamSetFunc)(void*, float);

typedef struct _tFiltModule {
    void* theFilt;
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncFloatInReturningFloat tick; // The object's tick function
    tFiltInternalParamSetFunc freq_set_func;
    tFiltInternalParamSetFunc gain_set_func;
    tFiltInternalParamSetFunc Q_set_func;
    tSetter setterFunctions[FiltNumParams]; // Array containing setter functions
    float* dbTableAddress;
    uint32_t dbTableScalar;
    float dbTableOffset;
    float dbTableSizeMinusOne;
    float* resTableAddress;
    float resTableSizeMinusOne;
    float params[FiltNumParams];
    float outputs[1];
    float amp;
    float cutoffKnob;
    float keyFollow;
    float inputNote;
    float sr;
    float invSr;
    tMempool mempool;
} _tFiltModule;

typedef _tFiltModule* tFiltModule;

//init module
void tFiltModule_init(void** const filt, float* const params, float id, LEAF* const leaf);
void tFiltModule_initToPool(void** const filt, float* const params, float id, tMempool* const mempool);
void tFiltModule_free(void** const filt);

// tick
void tFiltModule_tick (tFiltModule const filt);

//Modulatable setters
void tFiltModule_setMIDIPitch (tFiltModule const filt, float const input);
void tFiltModule_setCutoff(tFiltModule const filt, float cutoff);

// Non-modulatable setters

void tFiltModule_setDBtoATableLocation (tFiltModule const filt, float* tableAddress, uint32_t tableSize);
void tFiltModule_setResTableLocation (tFiltModule const filt, float* tableAddress, uint32_t tableSize);
void tFiltModule_setSampleRate (tFiltModule const filt, float sr);

//init processors
void tFiltModule_processorInit(tFiltModule const filt, leaf::tProcessor* processor);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif //SIMPLEOSCMODULE_H
//
// Created by Jeffrey Snyder on 7/31/24.
//

#ifndef FILTERMODULE_H
#define FILTERMODULE_H

#endif //FILTERMODULE_H
