//
// Created by Joshua Warner on 6/13/24.
//

#include "LFOModule.h"

#include <assert.h>

void tLFOModule_init(void** const lfo, float* params, float id, LEAF* const leaf)
{
    tLFOModule_initToPool(lfo, params, id,  &leaf->mempool);
}

void tLFOModule_blankFunction (tLFOModule const lfo, float freq)
{
    ;
}
void tLFOModule_initToPool(void** const lfo, float* const params, float id, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tLFOModule* LFOModule = *lfo = (_tLFOModule*) mpool_alloc(sizeof(_tLFOModule), m);
    memcpy(LFOModule->params, params, LFONumParams*sizeof(float));
    LFOModule->uniqueID = id;

    int type = roundf(LFOModule->params[LFOType]);
    LFOModule->mempool = m;
    LFOModule->setterFunctions[LFONoteOnWatchFlag] = &tLFOModule_blankFunction;
    LFOModule->setterFunctions[LFOType] = &tLFOModule_blankFunction;
    if (type == LFOTypeSineTri) {
        tSineTriLFO_initToPool((tSineTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tSineTriLFO_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tSineTriLFO_setShape;
        LFOModule->setterFunctions[LFORateParam] = &tSineTriLFO_setFreq;
        LFOModule->tick = &tSineTriLFO_tick;
    }
    else if (type == LFOTypeSawSquare) {

        tSawSquareLFO_initToPool((tSawSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tSawSquareLFO_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tSawSquareLFO_setShape;
        LFOModule->setterFunctions[LFORateParam] = &tSawSquareLFO_setFreq;
        LFOModule->tick = &tSawSquareLFO_tick;
    }
    else if (type == LFOTypeSine) {

        tCycle_initToPool((tCycle*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tCycle_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tCycle_setFreq;
        LFOModule->tick = &tCycle_tick;
    }
    else if (type == LFOTypeTri) {

        tTriLFO_initToPool((tTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tTriLFO_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tTriLFO_setFreq;
        LFOModule->tick = &tTriLFO_tick;
    }
    else if (type == LFOTypeSaw) {

        tIntPhasor_initToPool((tIntPhasor*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tIntPhasor_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tIntPhasor_setFreq;
        LFOModule->tick = &tIntPhasor_tickBiPolar;
    }
    else if (type == LFOTypeSquare) {

        tSquareLFO_initToPool((tSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tSquareLFO_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tSquareLFO_setPulseWidth;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tSquareLFO_setFreq;
        LFOModule->tick = &tSquareLFO_tick;
    }
    LFOModule->moduleType = ModuleTypeLFOModule;
}


void tLFOModule_free(void** const lfo)
{
    _tLFOModule* LFOModule = *lfo;
    int type = roundf(LFOModule->params[LFOType]);
    if (type == LFOTypeSineTri) {
        tSineTriLFO_free(LFOModule->theLFO);
    }
    else if (type == LFOTypeSawSquare) {
        tSawSquareLFO_free(LFOModule->theLFO);
    }
    else if (type == LFOTypeSine) {
        tCycle_free(LFOModule->theLFO);
    }
    else if (type == LFOTypeTri) {
        tTriLFO_free(LFOModule->theLFO);
    }
    else if (type == LFOTypeSaw) {
        tIntPhasor_free(LFOModule->theLFO);
    }
    else if (type == LFOTypeSquare) {
        tSquareLFO_free(LFOModule->theLFO);
    }
    mpool_free((char*)LFOModule, LFOModule->mempool);
}
//tick function
void tLFOModule_tick (tLFOModule const lfo)
{
    lfo->outputs[0] = lfo->tick(lfo->theLFO);
}

//special noteOnFunction
void tLFOModule_onNoteOn(tLFOModule const lfo, float pitch, float velocity)
{
    lfo->setterFunctions[LFOPhaseParam](lfo->theLFO, lfo->params[LFOPhaseParam]); //call actual function
}

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate)
{
    rate *= (float)lfo->rateTableSize;
    int inputInt = (int)rate;
    float inputFloat = (float)inputInt - rate;
    int nextPos = LEAF_clip(0, inputInt + 1, lfo->rateTableSize);
    float tempRate = (lfo->rateTable[inputInt] * (1.0f - inputFloat)) + (lfo->rateTable[nextPos] * inputFloat);
    lfo->setterFunctions[LFORateParam](lfo->theLFO, tempRate);
}

void tLFOModule_setShape(tLFOModule const lfo, float shape)
{
    lfo->setterFunctions[LFOShapeParam](lfo->theLFO, shape);
}

void tLFOModule_setPhase (tLFOModule const lfo, float phase)
{
    lfo->setterFunctions[LFOPhaseParam](lfo->theLFO, phase);
}

// Non-modulatable setters
void tLFOModule_setRateTableLocationAndSize (tLFOModule const lfo, float* tableAddress, uint32_t size)
{
    lfo->rateTable = tableAddress;
    lfo->rateTableSize = size-1;
}
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr)
{
    //how to handle this? if then cases for different types?

}

void tLFOModule_processorInit(tLFOModule const lfo, tProcessor* processor)
{
    // Checks that arguments are valid
	if (lfo == NULL)
	{
		return;
	}
    if (processor == NULL)
    {
    	return;
    }
    processor->processorUniqueID = lfo->uniqueID;
    processor->object = lfo;
    processor->numSetterFunctions = LFONumParams;
    processor->tick = &tLFOModule_tick;
    //memcpy(processor->setterFunctions, lfo->setterFunctions, LFONumParams*sizeof(void*));
    //write over the rate setter since it has some scaling
    processor->setterFunctions[LFORateParam] = &tLFOModule_setRate;
    processor->setterFunctions[LFOShapeParam] = &tLFOModule_setShape;
    processor->setterFunctions[LFOPhaseParam] = &tLFOModule_setPhase;
    processor->setterFunctions[LFONoteOnWatchFlag] = &tLFOModule_blankFunction;
    processor->setterFunctions[LFOType] = &tLFOModule_blankFunction;
    for (int i = 0; i < LFONumParams; i++)
    {
        processor->setterFunctions[i](lfo, lfo->params[i]);
    }
    processor->inParameters = lfo->params;
    processor->outParameters = lfo->outputs;
    processor->processorTypeID = ModuleTypeLFOModule;
}

