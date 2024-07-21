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
#include "../LEAF/leaf/Inc/leaf-oscillators.h"


typedef enum {
    OscMidiPitch,
    OscHarmonic,
    OscPitchOffset,
    OscPitchFine,
    OscFreqOffset,
    OscShapeParam,
    OscAmpParam,
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
    tTickFuncReturningFloat intTick; // The object's tick function
    tTickFuncReturningVoid extTick;
    tFreqSetFunc freq_set_func;
    tSetter setterFunctions[OscNumParams]; // Array containing setter functions
    float params[OscNumParams];
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
void tOscModule_init(void** const osc, float* const params, float id, LEAF* const leaf);
void tOscModule_initToPool(void** const osc, float* const params, float id, tMempool* const mempool);
void tOscModule_free(void** const osc);

// Modulatable setters
void tOscModule_tick (tOscModule const osc);


// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* tableAddress);
void tOscModule_setSampleRate (tOscModule const osc, float sr);

void tOscModule_setAllParams(tOscModule const osc);
//init processors
void tOscModule_processorInit(tOscModule const osc, tProcessor* processor);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif //SIMPLEOSCMODULE_H
