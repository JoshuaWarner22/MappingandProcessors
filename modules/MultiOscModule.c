//
// Created by Joshua Warner on 6/13/24.
//

#include "MultiOscModule.h"

#include <assert.h>

void tMultiOscModule_init(void** const osc, float* const params, LEAF* const leaf)
{
    tMultiOscModule_iniToPool(osc, params, &leaf->mempool);

}

void tMultiOscModule_iniToPool(void** const osc, float* const params, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tMultiOscModule* multiOsc = *osc = (_tMultiOscModule*) mpool_alloc(sizeof(_tMultiOscModule), m);

    multiOsc->mempool = m;
    tCycle_initToPool(&multiOsc->oscs[0], mempool);
    tCycle_initToPool(&multiOsc->oscs[1], mempool);
    tCycle_initToPool(&multiOsc->oscs[2], mempool);
    tCycle_initToPool(&multiOsc->oscs[3], mempool);
    multiOsc->moduleType = ModuleTypeMultiOscModule;
    //multiOsc->tick = &tMultiOscModule_tick;
}

void tMultiOscModule_free(void**  const osc)
{
    _tMultiOscModule* multiOscModule = (_tMultiOscModule*) *osc;

    tCycle_free(&multiOscModule->oscs[0]);
    tCycle_free(&multiOscModule->oscs[1]);
    tCycle_free(&multiOscModule->oscs[2]);
    tCycle_free(&multiOscModule->oscs[3]);

    mpool_free((char*)multiOscModule, multiOscModule->mempool);
}

// MultiOsc tick function
void tMultiOscModule_tick (tMultiOscModule osc)
{

    float finalFreq = osc->params[MultiOscFreq] * osc->params[MultiOscHarmonic];

    tCycle_setFreq(&osc->oscs[0], finalFreq);
    tCycle_setFreq(osc->oscs[1], finalFreq * 2);
    tCycle_setFreq(osc->oscs[2], finalFreq * 3);
    tCycle_setFreq(osc->oscs[3], finalFreq * 4);

    float sum = 0.f;

    sum += tCycle_tick(osc->oscs[0]) * 0.25;
    sum += tCycle_tick(osc->oscs[1]) * 0.25;
    sum += tCycle_tick(osc->oscs[2]) * 0.25;
    sum += tCycle_tick(osc->oscs[3]) * 0.25;

    osc->outputs[0] = sum * osc->params[MultiOscAmp];

}

// Modulatable setters
void tMultiOscModule_setFreq (tMultiOscModule osc, float freq)
{
    osc->params[MultiOscFreq] = freq;
}
void tMultiOscModule_setFineTune (tMultiOscModule osc, float ft)
{

    osc->params[MultiOscFineTune] = ft;
}
void tMultiOscModule_setHarmonic (tMultiOscModule osc, float harmonic)
{
   osc->params[MultiOscHarmonic] = harmonic;
}
void tMultiOscModule_setAmp (tMultiOscModule  osc, float amp)
{
    osc->params[MultiOscAmp] = amp;
}
void tMultiOscModule_setPhase (tMultiOscModule osc, float phase)
{
    //_tMultiOscModule * multiOscModule = *osc;
    osc->params[MultiOscPhase] = phase;

    tCycle_setPhase(osc->oscs[0], phase);
    tCycle_setPhase(osc->oscs[1], phase);
    tCycle_setPhase(osc->oscs[2], phase);
    tCycle_setPhase(osc->oscs[3], phase);
}

// Non-modulatable setters
void tMultiOscModule_setSampleRate (tMultiOscModule osc, float sr)
{

    tCycle_setSampleRate(osc->oscs[0], sr);
    tCycle_setSampleRate(osc->oscs[1], sr);
    tCycle_setSampleRate( osc->oscs[2], sr);
    tCycle_setSampleRate(osc->oscs[3], sr);
}

void tMultiOscModule_processorInit(tMultiOscModule osc, tProcessor* processor)
{
    // Checks that arguments are valid
    assert (osc != NULL);
    assert (processor != NULL);



    processor->processorUniqueID = osc->uniqueID;
    processor->object = osc;
    processor->tick = (tTickFuncReturningVoid) &tMultiOscModule_tick;

    processor->numSetterFunctions = MultiOscNumParams;
    processor->setterFunctions[MultiOscFreq] = (tSetter) &tMultiOscModule_setFreq;
    processor->setterFunctions[MultiOscFineTune] = (tSetter) &tMultiOscModule_setFineTune;
    processor->setterFunctions[MultiOscHarmonic] = (tSetter) &tMultiOscModule_setHarmonic;
    processor->setterFunctions[MultiOscAmp] = (tSetter) &tMultiOscModule_setAmp;
    processor->setterFunctions[MultiOscPhase] = (tSetter) &tMultiOscModule_setPhase;
    processor->inParameters = osc->params;
    processor->outParameters = osc->outputs;

    processor->processorTypeID = ModuleTypeMultiOscModule;
}