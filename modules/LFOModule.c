//
// Created by Joshua Warner on 6/13/24.
//

#include "LFOModule.h"

#include <assert.h>

void tLFOModule_init(void** const lfo, float* params, LEAF* const leaf)
{
    tLFOModule_initToPool(lfo, params, &leaf->mempool);
}
void tLFOModule_onNoteOn(void* const lfoV, float value)
{
    _tLFOModule* const lfo = (_tLFOModule*) lfoV;
    lfo->setterFunctions[LFOPhase](lfo->theLFO, lfo->params[LFOPhase]); //call actual function
}
void tLFOModule_blankFunction (tLFOModule const lfo, float freq)
{
    ;
}
void tLFOModule_initToPool(void** const lfo, float* const params, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tLFOModule* LFOModule = *lfo = (_tLFOModule*) mpool_alloc(sizeof(_tLFOModule), m);
    memcpy(LFOModule->params, params, LFONumParams);

    int shape = roundf(LFOModule->params[LFOShape]);
    LFOModule->mempool = m;
    LFOModule->setterFunctions[LFONoteOnWatchFlag] = &tLFOModule_onNoteOn;
    if (shape == LFOShapeSineTri) {
        tSineTriLFO_initToPool(LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhase] = &tSineTriLFO_setPhase;
        LFOModule->setterFunctions[LFOShape] = &tSineTriLFO_setShape;
        LFOModule->setterFunctions[LFORate] = &tSineTriLFO_setFreq;
        LFOModule->tick = tSineTriLFO_tick;
    }
    else if (shape == LFOShapeSawSquare) {
        tSawSquareLFO_initToPool(LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhase] = &tSawSquareLFO_setPhase;
        LFOModule->setterFunctions[LFOShape] = &tSawSquareLFO_setShape;
        LFOModule->setterFunctions[LFORate] = &tSawSquareLFO_setFreq;
        LFOModule->tick = &tSawSquareLFO_tick;
    }
    else if (shape == LFOShapeSine) {
        tCycle_initToPool(LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhase] = &tCycle_setPhase;
        LFOModule->setterFunctions[LFOShape] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORate] = &tCycle_setFreq;
        LFOModule->tick = &tCycle_tick;
    }
    else if (shape == LFOShapeTri) {
        tTriLFO_initToPool(LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhase] = &tTriLFO_setPhase;
        LFOModule->setterFunctions[LFOShape] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORate] = &tTriLFO_setFreq;
        LFOModule->tick = &tTriLFO_tick;
    }
    else if (shape == LFOShapeSaw) {
        tIntPhasor_initToPool(LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhase] = &tIntPhasor_setPhase;
        LFOModule->setterFunctions[LFOShape] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORate] = &tIntPhasor_setFreq;
        LFOModule->tick = &tIntPhasor_tick;
    }
    else if (shape == LFOShapeSquare) {
        tSquareLFO_initToPool(LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhase] = &tSquareLFO_setPhase;
        LFOModule->setterFunctions[LFOShape] = &tSquareLFO_setPulseWidth;//better way to do this?
        LFOModule->setterFunctions[LFORate] = &tSquareLFO_setFreq;
        LFOModule->tick = &tSquareLFO_tick;
    }

    LFOModule->moduleType = ModuleTypeLFOModule;
}


void tLFOModule_free(void** const lfo)
{
    _tLFOModule* LFOModule = *lfo;
    int shape = roundf(LFOModule->params[LFOShape]);
    if (shape == LFOShapeSineTri) {
        tSineTriLFO_free(LFOModule->theLFO);
    }
    else if (shape == LFOShapeSawSquare) {
        tSawSquareLFO_free(LFOModule->theLFO);
    }
    else if (shape == LFOShapeSine) {
        tCycle_free(LFOModule->theLFO);
    }
    else if (shape == LFOShapeTri) {
        tTriLFO_free(LFOModule->theLFO);
    }
    else if (shape == LFOShapeSaw) {
        tIntPhasor_free(LFOModule->theLFO);
    }
    else if (shape == LFOShapeSquare) {
        tSquareLFO_free(LFOModule->theLFO);
    }
    mpool_free((char*)LFOModule, LFOModule->mempool);
}

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate)
{
    rate *= 2047.0f;
    int inputInt = (int)rate;
    float inputFloat = (float)inputInt - rate;
    int nextPos = LEAF_clip(0, inputInt + 1, 2047);
    float tempRate = (lfo->rateTable[inputInt] * (1.0f - inputFloat)) + (lfo->rateTable[nextPos] * inputFloat);
    lfo->setterFunctions[LFORate](lfo->theLFO, tempRate);
}

// Non-modulatable setters
void tLFOModule_setRateTableLocation (tLFOModule const lfo, float* tableAddress)
{
    lfo->rateTable = tableAddress;
}
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr)
{
    //tCycle_setSampleRate(osc->oscs[0], sr);
}

void tLFOModule_processorInit(tLFOModule const lfo, tProcessor* processor)
{
    // Checks that arguments are valid
    assert (lfo != NULL);
    assert (processor != NULL);

    processor->processorUniqueID = lfo->uniqueID;
    processor->object = lfo;
    processor->numSetterFunctions = LFONumParams;
    processor->tick = (tTickFunc) *lfo->tick;
    memcpy(processor->setterFunctions, lfo->setterFunctions, LFONumParams);
    //write over the rate setter since it has some scaling
    processor->setterFunctions[LFORate] = &tLFOModule_setRate;
    processor->inParameters = lfo->params;
    processor->processorTypeID = ModuleTypeLFOModule;
}

