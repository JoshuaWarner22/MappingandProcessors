//
// Created by Jeffrey Snyder on 7/9/24.
//

#ifndef ENVMODULE_H
#define ENVMODULE_H

typedef enum {
    LFONoteOnWatchFlag,
    LFORate,
    LFOShape,
    LFOPhase,
    LFOType,//non modulatable
    LFONumParams
} LFOParams;

typedef enum {
    LFOTypeSineTri,
    LFOTypeSawSquare,
    LFOTypeSine,
    LFOTypeTri,
    LFOTypeSaw,
    LFOTypeSquare,
    LFONumTypes
} LFOTypes;

typedef struct _tLFOModule {
    void* theLFO;
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    tSetter setterFunctions[LFONumParams]; // Array containing setter functions
    float params[LFONumParams];
    float outputs[1];
    float* rateTable;
    tMempool mempool;
} _tLFOModule;

typedef _tLFOModule* tLFOModule;

//init module
void tLFOModule_init(void** const lfo, float* const params, LEAF* const leaf);
void tLFOModule_initToPool(void** const lfo, float* const params, tMempool* const mempool);
void tLFOModule_free(void** const lfo);

//note on action
void tLFOModule_onNoteOn(void* const lfoV, float value);

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate);

// Non-modulatable setters
void tLFOModule_setRateTableLocation (tLFOModule const lfo, float* tableAddress);
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr);

//init processor
void tLFOModule_processorInit(tLFOModule const lfo, tProcessor* processor);

#endif //ENVMODULE_H
