//
// Created by Joshua Warner on 6/13/24.
//

#include "LFOModule.h"

#include <assert.h>

void tLFOModule_init(void** const lfo, float* params, float id, LEAF* const leaf)
{
    if(leaf->lfoRateTable == NULL)
    {
        tLookupTable_init(&leaf->lfoRateTable, 0.3f, 30.f, 2.f,2048,leaf);
       // LEAF_generate_table_skew_non_sym(skewTableLFORate, 0.0f, 30.0f, 2.0f, 2048);
    }
    tLFOModule_initToPool(lfo, params, id,  &leaf->mempool, leaf->lfoRateTable);
}

void tLFOModule_blankFunction (tLFOModule const lfo, float freq)
{
    ;
}
void tLFOModule_initToPool(void** const lfo, float* const params, float id, tMempool* const mempool, tlookuptable rateTable)
{
    _tMempool* m = *mempool;
    _tLFOModule* LFOModule = static_cast<_tLFOModule*>(*lfo = (_tLFOModule*) mpool_alloc(sizeof(_tLFOModule), m));
    memcpy(LFOModule->params, params, LFONumParams*sizeof(float));
    LFOModule->uniqueID = id;
    LFOModule->table = rateTable;
    LFOModule->params[LFOType] = 0;
    int type = roundf(LFOModule->params[LFOType]);

    LFOModule->mempool = m;
    LFOModule->setterFunctions[LFONoteOnWatchFlag] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);
    LFOModule->setterFunctions[LFOType] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);
    LFOModule->setterFunctions[LFORateParam] = reinterpret_cast<tSetter>(&tLFOModule_setRate);
    if (type == LFOTypeSineTri) {
        tSineTriLFO_initToPool((tSineTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tSineTriLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tSineTriLFO_setShape);
        LFOModule->freq_set_func = reinterpret_cast<tSetter>(&tSineTriLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tSineTriLFO_tick);
    }
    else if (type == LFOTypeSawSquare) {

        tSawSquareLFO_initToPool((tSawSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tSawSquareLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tSawSquareLFO_setShape);
        LFOModule->freq_set_func = reinterpret_cast<tSetter>(&tSawSquareLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tSawSquareLFO_tick);
    }
    else if (type == LFOTypeSine) {

        tCycle_initToPool((tCycle*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tCycle_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);//better way to do this?
        LFOModule->freq_set_func = reinterpret_cast<tSetter>(&tCycle_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tCycle_tick);
    }
    else if (type == LFOTypeTri) {

        tTriLFO_initToPool((tTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tTriLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);//better way to do this?
        LFOModule->freq_set_func  = reinterpret_cast<tSetter>(&tTriLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tTriLFO_tick);
    }
    else if (type == LFOTypeSaw) {

        tIntPhasor_initToPool((tIntPhasor*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tIntPhasor_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);//better way to do this?
        LFOModule->freq_set_func = reinterpret_cast<tSetter>(&tIntPhasor_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tIntPhasor_tickBiPolar);
    }
    else if (type == LFOTypeSquare) {

        tSquareLFO_initToPool((tSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tSquareLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tSquareLFO_setPulseWidth);//better way to do this?
        LFOModule->freq_set_func = reinterpret_cast<tSetter>(&tSquareLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat >(&tSquareLFO_tick);
    }
    LFOModule->moduleType = ModuleTypeLFOModule;
}


void tLFOModule_free(void** const lfo)
{
    _tLFOModule* LFOModule = static_cast<_tLFOModule*>(*lfo);
    int type = roundf(LFOModule->params[LFOType]);
    if (type == LFOTypeSineTri) {
        tSineTriLFO_free((tSineTriLFO*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSawSquare) {
        tSawSquareLFO_free((tSawSquareLFO*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSine) {
        tCycle_free((tCycle*)LFOModule->theLFO);
    }
    else if (type == LFOTypeTri) {
        tTriLFO_free((tTriLFO*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSaw) {
        tIntPhasor_free((tIntPhasor*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSquare) {
        tSquareLFO_free((tSquareLFO*)LFOModule->theLFO);
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
    rate *= (float)lfo->table->tableSize;
    int inputInt = (int)rate;
    float inputFloat = (float)inputInt - rate;
    int nextPos = LEAF_clip(0, inputInt + 1, lfo->table->tableSize);
    float tempRate = (lfo->table->table[inputInt] * (1.0f - inputFloat)) + (lfo->table->table[nextPos] * inputFloat);
    lfo->freq_set_func (lfo->theLFO, tempRate);
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
//void tLFOModule_setRateTableLocationAndSize (tLFOModule const lfo, float* tableAddress, uint32_t size)
//{
//    lfo->rateTable = tableAddress;
//    lfo->rateTableSize = size-1;
//}
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr)
{
    //how to handle this? if then cases for different types?

}

void tLFOModule_processorInit(tLFOModule const lfo, leaf::tProcessor* processor)
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
    processor->tick = reinterpret_cast<tTickFuncReturningVoid>(&tLFOModule_tick);
    //memcpy(processor->setterFunctions, lfo->setterFunctions, LFONumParams*sizeof(void*));
    //write over the rate setter since it has some scaling
    processor->setterFunctions[LFORateParam] = reinterpret_cast<tSetter>(&tLFOModule_setRate);
    processor->setterFunctions[LFOShapeParam] = reinterpret_cast<tSetter>(&tLFOModule_setShape);
    processor->setterFunctions[LFOPhaseParam] = reinterpret_cast<tSetter>(&tLFOModule_setPhase);
    processor->setterFunctions[LFONoteOnWatchFlag] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);
    processor->setterFunctions[LFOType] = reinterpret_cast<tSetter>(&tLFOModule_blankFunction);
    for (int i = 0; i < LFONumParams; i++)
    {
        processor->setterFunctions[i](lfo, lfo->params[i]);
    }
    processor->inParameters = lfo->params;
    processor->outParameters = lfo->outputs;
    processor->processorTypeID = ModuleTypeLFOModule;
}

