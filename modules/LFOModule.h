//
// Created by Joshua Warner on 6/13/24.
//

#ifndef LFOMODULE_H
#define LFOMODULE_H

#include "../LEAF/leaf/leaf.h"
#include "../defs.h"
#include "../processor.h"
#include "../LEAF/leaf/Inc/leaf-mempool.h"
#include "../LEAF/leaf/Inc/leaf-oscillators.h"


typedef enum {
    LFONoteOnWatchFlag,//this aliases the sync param
    LFORate,
    LFOShape,
    LFOPhase,
    LFOType,//non modulatable
    LFONumParams
} LFOParams;

typedef enum {
    LFOShapeSineTri,
    LFOShapeSawSquare,
    LFOShapeSine,
    LFOShapeTri,
    LFOShapeSaw,
    LFOShapeSquare,
    LFONumShapes
} LFOShapes;

typedef struct _tLFOModule {
   void* theLFO;
   uint8_t moduleType;
   uint8_t uniqueID;
    tTickFunc tick; // The object's tick function
    tSetter setterFunctions[LFONumParams]; // Array containing
    tSetter skewedRateSetter;
   float params[LFONumParams];
   float outputs[1];
    float* rateTable;
   tMempool mempool;
} _tLFOModule;

typedef _tLFOModule* tLFOModule;

void tLFOModule_init(void** const lfo, float* const params, LEAF* const leaf);
void tLFOModule_initToPool(void** const lfo, float* const params, tMempool* const mempool);
void tLFOModule_free(void** const lfo);

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate);

// Non-modulatable setters
void tLFOModule_setRateTableLocation (tLFOModule const lfo, float* tableAddress);
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr);
void tLFOModule_processorInit(tLFOModule const lfo, tProcessor* processor);






#endif //LFOMODULE_H
