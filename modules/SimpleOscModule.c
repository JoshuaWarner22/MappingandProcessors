//
// Created by Joshua Warner on 6/13/24.
//

#include "SimpleOscModule.h"

#include <assert.h>

void tOscModule_init(void** const osc, float* params, LEAF* const leaf)
{
    tOscModule_initToPool(osc, params, &leaf->mempool);
}

void tOscModule_blankFunction (tOscModule const osc, float freq)
{
    ;
}
void tOscModule_initToPool(void** const osc, float* const params, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tOscModule* OscModule = *osc = (_tOscModule*) mpool_alloc(sizeof(_tOscModule), m);
    memcpy(OscModule->params, params, OscNumParams);

    int type = roundf(OscModule->params[OscType]);
    OscModule->mempool = m;
    OscModule->setterFunctions[OscNoteOnWatchFlag] = &tOscModule_blankFunction;

   //fix this when expsmooth is fixed to new style
    tExpSmooth_initToPool(&OscModule->pitchSmoother, 64.0f, 0.05f, &m);
    if (type == OscTypeSawSquare) {
        tPBSawSquare_initToPool((tPBSawSquare*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = tPBSawSquare_setFreq;
        OscModule->setterFunctions[OscShape] = tPBSawSquare_setShape;
        OscModule->tick = tPBSawSquare_tick;
    }
    else if (type == OscTypeSineTri) {
        //OscModule->theOsc = (_tPBSineTriangle*) mpool_alloc(sizeof(_tPBSineTriangle), m);
        tPBSineTriangle_initToPool(OscModule->theOsc, mempool);
        OscModule->freq_set_func = tPBSineTriangle_setFreq;
        OscModule->setterFunctions[OscShape] = tPBSineTriangle_setShape;
        OscModule->tick = tPBSineTriangle_tick;
    }
    else if (type == OscTypeSaw) {
        //OscModule->theOsc = (_tPBSaw*) mpool_alloc(sizeof(_tPBSaw), m);
        tPBSaw_initToPool(OscModule->theOsc, mempool);
        OscModule->freq_set_func = tPBSaw_setFreq;
        OscModule->setterFunctions[OscShape] = tOscModule_blankFunction;
        OscModule->tick = tPBSaw_tick;
    }
    else if (type == OscTypePulse) {
        //OscModule->theOsc = (_tPBPulse*) mpool_alloc(sizeof(_tPBPulse), m);
        tPBPulse_initToPool(OscModule->theOsc, mempool);
        OscModule->freq_set_func = tPBPulse_setFreq;
        OscModule->setterFunctions[OscShape] = tPBPulse_setWidth;
        OscModule->tick = tPBPulse_tick;
    }
    else if (type == OscTypeSine) {
        //OscModule->theOsc = (_tCycle*) mpool_alloc(sizeof(_tCycle), m);
        tCycle_initToPool(OscModule->theOsc, mempool);
        OscModule->freq_set_func = tCycle_setFreq;
        OscModule->setterFunctions[OscShape] = tOscModule_blankFunction;
        OscModule->tick = tCycle_tick;
    }
    else if (type == OscTypeTri) {
        //OscModule->theOsc = (_tPBTriangle*) mpool_alloc(sizeof(_tPBTriangle), m);
        tPBTriangle_initToPool(OscModule->theOsc, mempool);
        OscModule->freq_set_func = tPBTriangle_setFreq;
        OscModule->setterFunctions[OscShape] = tPBTriangle_setSkew;
        OscModule->tick = tPBTriangle_tick;
    }
    OscModule->moduleType = ModuleTypeOscModule;
}


void tOscModule_free(void** const lfo)
{
    _tOscModule* OscModule = *lfo;
    int type = roundf(OscModule->params[OscType]);
    if (type == OscTypeSawSquare) {
        tPBSawSquare_free(OscModule->theOsc);
    }
    else if (type == OscTypeSineTri) {
        tPBSineTriangle_free(OscModule->theOsc);
    }
    else if (type == OscTypeSaw) {
        tPBSaw_free(OscModule->theOsc);
    }
    else if (type == OscTypePulse) {
        tPBPulse_free(OscModule->theOsc);
    }
    else if (type == OscTypeSine) {
        tCycle_free(OscModule->theOsc);
    }
    else if (type == OscTypeTri) {
        tPBTriangle_free(OscModule->theOsc);
    }
    tExpSmooth_free(&OscModule->pitchSmoother);
    mpool_free((char*)OscModule, OscModule->mempool);
}


// tick function
void tOscModule_tick (tOscModule const osc)
{
    float freqToSmooth = (osc->inputNote + (osc->fine));
    tExpSmooth_setDest(&osc->pitchSmoother, freqToSmooth);
    float tempMIDI = tExpSmooth_tick(&osc->pitchSmoother) + osc->pitchOffset + osc->octaveOffset;

    float tempIndexF = ((LEAF_clip(-163.0f, tempMIDI, 163.0f) * 100.0f) + 16384.0f);
    int tempIndexI = (int)tempIndexF;
    tempIndexF = tempIndexF -tempIndexI;
    float freqToSmooth1 = osc->mtofTable[tempIndexI & 32767];
    float freqToSmooth2 = osc->mtofTable[(tempIndexI + 1) & 32767];
    float nowFreq =  ((freqToSmooth1 * (1.0f - tempIndexF)) + (freqToSmooth2 * tempIndexF));

    float finalFreq = (nowFreq * osc->harmonicMultiplier) + osc->freqOffset;
    osc->freq_set_func(osc->theOsc, finalFreq);
    osc->outputs[0] = osc->tick(osc->theOsc) * osc->amp;
}


// Modulatable setters

void tOscModule_setHarmonic (tOscModule const osc, float harm)
{
    harm *= 15.0f;
    if (osc->stepped)
    {
        harm = roundf(harm);
    }

    if (harm >= 0.0f)
    {
        osc->harmonicMultiplier = (harm + 1.0f);
    }
    else
    {
        osc->harmonicMultiplier = (1.0f / fabsf((harm - 1.0f)));
    }
}

void tOscModule_setPitch (tOscModule const osc, float pitch)
{
    pitch *= 24.0f;
    if (osc->stepped)
    {
        pitch = roundf(pitch);
    }
    osc->pitchOffset = pitch;
}

void tOscModule_setFine (tOscModule const osc, float fine)
{
    osc->fine = fine * 2.0f - 1.0f;
}

void tOscModule_setFreq (tOscModule const osc, float freq)
{
    osc->freqOffset = (freq * 4000.0f) - 2000.0f;
}
void tOscModule_setOctave (tOscModule const osc, float oct)
{
    osc->octaveOffset = (roundf((oct * 6.0f) - 3.0f)) * 12.0f;
}
void tOscModule_setAmp (tOscModule const osc, float amp)
{
    osc->amp = amp;
}
void tOscModule_setGlide (tOscModule const osc, float glide)
{
    float factor = 1.0f;
    if (glide >= 0.00001f) {
        factor = 1.0f - (expf(-osc->invSr / glide));
    }
    tExpSmooth_setFactor(&osc->pitchSmoother,factor);
}
void tOscModule_setStepped (tOscModule const osc, float stepped)
{
    osc->stepped = roundf(stepped);
}

void tOscModule_setSyncMode (tOscModule const osc, float syncMode)
{
    osc->syncMode = roundf(syncMode);
}

void tOscModule_setSyncIn (tOscModule const osc, float syncIn)
{
    //call the per-type sync functions if they exist
}

// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* tableAddress)
{
    osc->mtofTable = tableAddress;
}
void tOscModule_setSampleRate (tOscModule const osc, float sr)
{
    //tCycle_setSampleRate(osc->oscs[0], sr);
}

void tOscModule_processorInit(tOscModule const osc, tProcessor* processor)
{
    // Checks that arguments are valid
    assert (osc != NULL);
    assert (processor != NULL);

    processor->processorUniqueID = osc->uniqueID;
    processor->object = osc;
    processor->numSetterFunctions = OscNumParams;
    processor->tick = &tOscModule_tick;
    processor->setterFunctions[OscNoteOnWatchFlag] = &tOscModule_blankFunction;
    processor->setterFunctions[OscHarmonic] = &tOscModule_setHarmonic;
    processor->setterFunctions[OscPitch] = &tOscModule_setPitch;
    processor->setterFunctions[OscFine] = &tOscModule_setFine;
    processor->setterFunctions[OscFreq] = &tOscModule_setFreq;
    processor->setterFunctions[OscShape] = osc->setterFunctions[OscShape];
    processor->setterFunctions[OscAmp] = &tOscModule_setAmp;
    processor->setterFunctions[OscGlide] = &tOscModule_setGlide;
    processor->setterFunctions[OscStepped] = &tOscModule_setStepped;
    processor->setterFunctions[OscSyncMode] = &tOscModule_setSyncMode;
    processor->setterFunctions[OscSyncIn] = &tOscModule_setSyncIn;
    processor->setterFunctions[OscType] = &tOscModule_blankFunction;
    for (int i = 0; i < OscNumParams; i++)
    {
        processor->setterFunctions[i](osc, osc->params[i]);
    }
    processor->inParameters = osc->params;
    processor->outParameters = osc->outputs;
    processor->processorTypeID = ModuleTypeOscModule;
}

