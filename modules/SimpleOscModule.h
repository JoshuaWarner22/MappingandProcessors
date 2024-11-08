//
// Created by Joshua Warner on 6/13/24.
//

#ifndef SIMPLEOSCMODULE_H
#define SIMPLEOSCMODULE_H
#include "../LEAF/leaf/leaf.h"
#include "../processors/defs.h"
#include "../processors/processor.h"
#include "../LEAF/leaf/Inc/leaf-mempool.h"
#include "../LEAF/leaf/Inc/leaf-oscillators.h"
#include <atomic>

typedef enum {
    OscMidiPitch,
    OscHarmonic,
    OscPitchOffset,
    OscPitchFine,
    OscFreqOffset,
    OscShapeParam,
    OscAmpParam,
    OscGlide,
    OscSteppedHarmonic,
    OscSteppedPitch,
    OscSyncMode,
    OscSyncIn,
    OscType,
    OscNumParams
} OscParams;

typedef enum {
    OscTypeSawSquare,
    OscTypeSineTri,
    OscTypeSaw,
    OscTypePulse,
    OscTypeSine,
    OscTypeTri,
    OscNumTypes
} OscTypes;

//the actual frequency setter function
typedef void (*tFreqSetFunc)(void*, float);

typedef struct _tOscModule {
    void* theOsc;
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    tFreqSetFunc freq_set_func;
    tSetter setterFunctions[OscNumParams]; // Array containing setter functions
    std::atomic<float> params[OscNumParams];
    std::atomic<float> outputs[1];
    float fine;
    float harmonicMultiplier;
    float pitchOffset;
    float freqOffset;
    tExpSmooth pitchSmoother;
    float octaveOffset;
    float inputNote;
    float finalFreq;
    float amp;
    float* mtofTable;
    float sr;
    float invSr;
    int hStepped;
    int pStepped;
    int syncMode;
    tMempool mempool;
} _tOscModule;

typedef _tOscModule* tOscModule;

//init module
void tOscModule_init(void** const osc, float* const params, float id, LEAF* const leaf);
void tOscModule_initToPool(void** const osc, float* const params, float id, tMempool* const mempool);
void tOscModule_free(void** const osc);

// Modulatable setters
void tOscModule_tick (tOscModule const osc, float*);


// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* tableAddress);
void tOscModule_setSampleRate (tOscModule const osc, float sr);

//init processors
void tOscModule_processorInit(tOscModule const osc, leaf::tProcessor* processor);

#endif //SIMPLEOSCMODULE_H
