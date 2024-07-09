//
// Created by Joshua Warner on 6/13/24.
//

#ifndef SIMPLEOSCMODULE_H
#define SIMPLEOSCMODULE_H

#include "../LEAF/leaf/leaf.h"
#include "../defs.h"
#include "../processor.h"
#include "../LEAF/leaf/Inc/leaf-mempool.h"
#include "../LEAF/leaf/Inc/leaf-oscillators.h"


typedef enum {
    OscNoteOnWatchFlag,
    OscMidiPitch,
    OscHarmonic,
    OscPitchTranspose,
    OscFine,
    OscFreq,
    OscShape,
    OscAmp,
    OscGlide,
    OscStepped,
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
    float params[OscNumParams];
    float scaledParams[OscNumParams];
    float outputs[1];
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
    int stepped;
    int syncMode;
    tMempool mempool;
} _tOscModule;

typedef _tOscModule* tOscModule;

//init module
void tOscModule_init(void** const osc, float* const params, LEAF* const leaf);
void tOscModule_initToPool(void** const osc, float* const params, tMempool* const mempool);
void tOscModule_free(void** const osc);

// Modulatable setters
void tOscModule_setFreq (tOscModule const osc, float rate);

void tOscModule_tick (tOscModule const osc);


// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* tableAddress);
void tOscModule_setSampleRate (tOscModule const osc, float sr);

//init processor
void tOscModule_processorInit(tOscModule const osc, tProcessor* processor);






#endif //SIMPLEOSCMODULE_H
