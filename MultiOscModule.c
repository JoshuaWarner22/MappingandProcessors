//
// Created by Joshua Warner on 6/13/24.
//

#include "MultiOscModule.h"

#include <assert.h>

void tMultiOscModule_init(tMultiOscModule* const osc, LEAF* const leaf)
{
    tMultiOscModule_iniToPool(osc, &leaf->mempool);
}

void tMultiOscModule_iniToPool(tMultiOscModule* const osc, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tMultiOscModule* multiOsc = *osc = (_tMultiOscModule*) mpool_alloc(sizeof(_tMultiOscModule), m);

    multiOsc->mempool = m;
    tCycle_initToPool(&multiOsc->oscs[0], mempool);
    tCycle_initToPool(&multiOsc->oscs[1], mempool);
    tCycle_initToPool(&multiOsc->oscs[2], mempool);
    tCycle_initToPool(&multiOsc->oscs[3], mempool);
}

void tMultiOscModule_free(tMultiOscModule* const osc)
{
    _tMultiOscModule* multiOscModule = *osc;

    tCycle_free(&multiOscModule->oscs[0]);
    tCycle_free(&multiOscModule->oscs[1]);
    tCycle_free(&multiOscModule->oscs[2]);
    tCycle_free(&multiOscModule->oscs[3]);

    mpool_free((char*)multiOscModule, multiOscModule->mempool);
}

// MultiOsc tick function
float tMultiOscModule_tick (tMultiOscModule* const osc)
{
    _tMultiOscModule * multiOscModule = *osc;
    float finalFreq = multiOscModule->params[MultiOscFreq] * multiOscModule->params[MultiOscHarmonic];

    tCycle_setFreq(multiOscModule->oscs[0], finalFreq);
    tCycle_setFreq(multiOscModule->oscs[1], finalFreq * 2);
    tCycle_setFreq(multiOscModule->oscs[2], finalFreq * 3);
    tCycle_setFreq(multiOscModule->oscs[3], finalFreq * 4);

    multiOscModule->outputs[MultiOscFreq] = finalFreq;

    float sum = 0.f;

    sum += tCycle_tick(multiOscModule->oscs[0]) * 0.25;
    sum += tCycle_tick(multiOscModule->oscs[1]) * 0.25;
    sum += tCycle_tick(multiOscModule->oscs[2]) * 0.25;
    sum += tCycle_tick(multiOscModule->oscs[3]) * 0.25;

    return sum * multiOscModule->params[MultiOscAmp];

}

// Modulatable setters
void tMultiOscModule_setFreq (tMultiOscModule* const osc, float freq)
{
    _tMultiOscModule * multiOscModule = *osc;
    multiOscModule->params[MultiOscFreq] = freq;
}
void tMultiOscModule_setFineTune (tMultiOscModule* const osc, float ft)
{
    _tMultiOscModule * multiOscModule = *osc;
    multiOscModule->params[MultiOscFineTune] = ft;
}
void tMultiOscModule_setHarmonic (tMultiOscModule* const osc, float harmonic)
{
    _tMultiOscModule * multiOscModule = *osc;
    multiOscModule->params[MultiOscHarmonic] = harmonic;
}
void tMultiOscModule_setAmp (tMultiOscModule* const osc, float amp)
{
    _tMultiOscModule * multiOscModule = *osc;
    multiOscModule->params[MultiOscAmp] = amp;
}
void tMultiOscModule_setPhase (tMultiOscModule* const osc, float phase)
{
    _tMultiOscModule * multiOscModule = *osc;
    multiOscModule->params[MultiOscPhase] = phase;

    tCycle_setPhase(multiOscModule->oscs[0], phase);
    tCycle_setPhase(multiOscModule->oscs[1], phase);
    tCycle_setPhase(multiOscModule->oscs[2], phase);
    tCycle_setPhase(multiOscModule->oscs[3], phase);
}

// Non-modulatable setters
void tMultiOscModule_setSampleRate (tMultiOscModule* const osc, float sr)
{
    _tMultiOscModule * multiOscModule = *osc;
    tCycle_setSampleRate(multiOscModule->oscs[0], sr);
    tCycle_setSampleRate(multiOscModule->oscs[1], sr);
    tCycle_setSampleRate(multiOscModule->oscs[2], sr);
    tCycle_setSampleRate(multiOscModule->oscs[3], sr);
}

void tMultiOscModule_processorInit(tMultiOscModule* const osc, tProcessor* processor)
{
    // Checks that arguments are valid
    assert (osc != NULL);
    assert (processor != NULL);

    _tMultiOscModule * multiOscModule = *osc;

    processor->processorUniqueID = multiOscModule->uniqueID;
    processor->object = osc;
    processor->tick = (tTickFunc) &tMultiOscModule_tick;

    processor->numSetterFunctions = MultiOscNumParams;
    processor->setterFunctions[MultiOscFreq] = (tSetter) &tMultiOscModule_setFreq;
    processor->setterFunctions[MultiOscFineTune] = (tSetter) &tMultiOscModule_setFineTune;
    processor->setterFunctions[MultiOscHarmonic] = (tSetter) &tMultiOscModule_setHarmonic;
    processor->setterFunctions[MultiOscAmp] = (tSetter) &tMultiOscModule_setAmp;
    processor->setterFunctions[MultiOscPhase] = (tSetter) &tMultiOscModule_setPhase;
    processor->inParameters = &multiOscModule->params;

    processor->processorTypeID = ModuleTypeMultiOscModule;
}