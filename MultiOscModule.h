//
// Created by Joshua Warner on 6/13/24.
//

#ifndef UNTITLED_MULTIOSCMODULE_H
#define UNTITLED_MULTIOSCMODULE_H

#include "LEAF/leaf/leaf.h"
#include "defs.h"
#include "processor.h"
#include "LEAF/leaf/Inc/leaf-mempool.h"
#include "LEAF/leaf/Inc/leaf-oscillators.h"

typedef enum {
    MultiOscFreq,
    MultiOscFineTune,
    MultiOscHarmonic,
    MultiOscAmp,
    MultiOscPhase,
    MultiOscNumParams
} MultiOscParams;

typedef struct _MultiOscModule {
   tCycle oscs[4];
   ModuleType moduleType;
   uint8_t uniqueID;

   tMempool mempool;

   tTickFunc tick; // The object's tick function
   //tSetter setterFunctions[MultiOscNumParams];

   float params[MultiOscNumParams];
   float outputs[MultiOscNumParams];
} _tMultiOscModule;

typedef _tMultiOscModule* tMultiOscModule;

void tMultiOscModule_init(tMultiOscModule* const osc, LEAF* const leaf);
void tMultiOscModule_iniToPool(tMultiOscModule* const osc, tMempool* const mempool);
void tMultiOscModule_free(tMultiOscModule* const osc);

// MultiOsc tick function
float tMultiOscModule_tick (tMultiOscModule* const osc);

// Modulatable setters
void tMultiOscModule_setFreq (tMultiOscModule* const osc, float freq);
void tMultiOscModule_setFineTune (tMultiOscModule* const osc, float ft);
void tMultiOscModule_setHarmonic (tMultiOscModule* const osc, float harmonic);
void tMultiOscModule_setAmp (tMultiOscModule* const osc, float amp);
void tMultiOscModule_setPhase (tMultiOscModule* const osc, float phase);

// Non-modulatable setters
void tMultiOscModule_setSampleRate (tMultiOscModule* const osc, float sr);

// Initializer function
void tMultiOscModule_processorInit(tMultiOscModule* const osc, tProcessor* processor);






///TODO
// write setters
// write tick()


#endif //UNTITLED_MULTIOSCMODULE_H
