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


//tick function
void tLFOModule_tick (tLFOModule const lfo)
{
    lfo->outputs[0] = lfo->intTick(lfo->theLFO);
}

//special noteOnFunction
void tLFOModule_onNoteOn(tLFOModule const lfo, float pitch, float velocity)
{
    lfo->intPhaseSetter(lfo->theLFO, lfo->params[LFOPhaseParam]); //call actual function
}

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate)
{
    rate *= (float)lfo->rateTableSize;
    int inputInt = (int)rate;
    float inputFloat = (float)inputInt - rate;
    int nextPos = LEAF_clip(0, inputInt + 1, lfo->rateTableSize);
    float tempRate = (lfo->rateTable[inputInt] * (1.0f - inputFloat)) + (lfo->rateTable[nextPos] * inputFloat);
    lfo->intRateSetter(lfo->theLFO, tempRate);
}

void tLFOModule_setShape(tLFOModule const lfo, float shape)
{
    lfo->intShapeSetter(lfo->theLFO, shape);
}

void tLFOModule_setPhase (tLFOModule const lfo, float phase)
{
    lfo->intPhaseSetter(lfo->theLFO, phase);
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
void tLFOModule_setAllParams (tLFOModule const lfo)
{
	for (int i = 0; i < LFONumParams; i++)
	{
		lfo->setterFunctions[i](lfo, lfo->params[i]);
	}
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
        LFOModule->intPhaseSetter = &tSineTriLFO_setPhase;
        LFOModule->intShapeSetter = &tSineTriLFO_setShape;
        LFOModule->intRateSetter = &tSineTriLFO_setFreq;
        LFOModule->intTick = &tSineTriLFO_tick;
    }
    else if (type == LFOTypeSawSquare) {

        tSawSquareLFO_initToPool((tSawSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->intPhaseSetter = &tSawSquareLFO_setPhase;
        LFOModule->intShapeSetter = &tSawSquareLFO_setShape;
        LFOModule->intRateSetter  = &tSawSquareLFO_setFreq;
        LFOModule->intTick = &tSawSquareLFO_tick;
    }
    else if (type == LFOTypeSine) {

        tCycle_initToPool((tCycle*)&LFOModule->theLFO, mempool);
        LFOModule->intPhaseSetter = &tCycle_setPhase;
        LFOModule->intShapeSetter = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->intRateSetter  = &tCycle_setFreq;
        LFOModule->intTick = &tCycle_tick;
    }
    else if (type == LFOTypeTri) {

        tTriLFO_initToPool((tTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tTriLFO_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tTriLFO_setFreq;
        LFOModule->intTick = &tTriLFO_tick;
    }
    else if (type == LFOTypeSaw) {

        tIntPhasor_initToPool((tIntPhasor*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tIntPhasor_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tLFOModule_blankFunction;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tIntPhasor_setFreq;
        LFOModule->intTick = &tIntPhasor_tickBiPolar;
    }
    else if (type == LFOTypeSquare) {

        tSquareLFO_initToPool((tSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = &tSquareLFO_setPhase;
        LFOModule->setterFunctions[LFOShapeParam] = &tSquareLFO_setPulseWidth;//better way to do this?
        LFOModule->setterFunctions[LFORateParam] = &tSquareLFO_setFreq;
        LFOModule->intTick = &tSquareLFO_tick;
    }

    LFOModule->extTick = &tLFOModule_tick;
    //memcpy(processor->setterFunctions, lfo->setterFunctions, LFONumParams*sizeof(void*));
    //write over the rate setter since it has some scaling
    LFOModule->setterFunctions[LFORateParam] = &tLFOModule_setRate;
    LFOModule->setterFunctions[LFOShapeParam] = &tLFOModule_setShape;
    LFOModule->setterFunctions[LFOPhaseParam] = &tLFOModule_setPhase;
    LFOModule->setterFunctions[LFONoteOnWatchFlag] = &tLFOModule_blankFunction;
    LFOModule->setterFunctions[LFOType] = &tLFOModule_blankFunction;

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

