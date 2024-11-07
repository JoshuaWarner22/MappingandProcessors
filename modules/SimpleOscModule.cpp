//
// Created by Joshua Warner on 6/13/24.
//

#include "SimpleOscModule.h"

#include <assert.h>

void tOscModule_init(void** const osc, float* params, float id, LEAF* const leaf)
{
    tOscModule_initToPool(osc, params, id, &leaf->mempool);
}

void tOscModule_blankFunction (tOscModule const osc, float freq)
{
    ;
}
void tOscModule_initToPool(void** const osc, float* const params, float id, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tOscModule* OscModule = static_cast<_tOscModule*> (*osc = (_tOscModule*) mpool_alloc (sizeof (_tOscModule), m));
    memcpy(OscModule->params, params, OscNumParams*sizeof(float));
    OscModule->uniqueID = id;

    int type = roundf(OscModule->params[OscType]);
    OscModule->mempool = m;

    tExpSmooth_initToPool(&OscModule->pitchSmoother, 64.0f, 0.05f, &m);
    if (type == OscTypeSawSquare) {
        tPBSawSquare_initToPool((tPBSawSquare*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = reinterpret_cast<tFreqSetFunc> (tPBSawSquare_setFreq);
        OscModule->setterFunctions[OscShapeParam] = reinterpret_cast<tSetter> (tPBSawSquare_setShape);
        OscModule->tick = reinterpret_cast<tTickFuncReturningFloat> (tPBSawSquare_tick);
    }
    else if (type == OscTypeSineTri) {
        tPBSineTriangle_initToPool((tPBSineTriangle*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = reinterpret_cast<tFreqSetFunc> (tPBSineTriangle_setFreq);
        OscModule->setterFunctions[OscShapeParam] = reinterpret_cast<tSetter> (tPBSineTriangle_setShape);
        OscModule->tick = reinterpret_cast<tTickFuncReturningFloat> (tPBSineTriangle_tick);
    }
    else if (type == OscTypeSaw) {
        tPBSaw_initToPool((tPBSaw*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = reinterpret_cast<tFreqSetFunc> (tPBSaw_setFreq);
        OscModule->setterFunctions[OscShapeParam] = reinterpret_cast<tSetter> (tOscModule_blankFunction);
        OscModule->tick = reinterpret_cast<tTickFuncReturningFloat> (tPBSaw_tick);
    }
    else if (type == OscTypePulse) {
        tPBPulse_initToPool((tPBPulse*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = reinterpret_cast<tFreqSetFunc> (tPBPulse_setFreq);
        OscModule->setterFunctions[OscShapeParam] = reinterpret_cast<tSetter> (tPBPulse_setWidth);
        OscModule->tick = reinterpret_cast<tTickFuncReturningFloat> (tPBPulse_tick);
    }
    else if (type == OscTypeSine) {
        tCycle_initToPool((tCycle*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = reinterpret_cast<tFreqSetFunc> (tCycle_setFreq);
        OscModule->setterFunctions[OscShapeParam] = reinterpret_cast<tSetter> (tOscModule_blankFunction);
        OscModule->tick = reinterpret_cast<tTickFuncReturningFloat> (tCycle_tick);
    }
    else if (type == OscTypeTri) {
        tPBTriangle_initToPool((tPBTriangle*)&OscModule->theOsc, mempool);
        OscModule->freq_set_func = reinterpret_cast<tFreqSetFunc> (tPBTriangle_setFreq);
        OscModule->setterFunctions[OscShapeParam] = reinterpret_cast<tSetter> (tPBTriangle_setSkew);
        OscModule->tick = reinterpret_cast<tTickFuncReturningFloat> (tPBTriangle_tick);
    }
    OscModule->moduleType = ModuleTypeOscModule;
}


void tOscModule_free(void** const osc)
{
    _tOscModule* OscModule = static_cast<_tOscModule*> (*osc);
    int type = roundf(OscModule->params[OscType]);
    if (type == OscTypeSawSquare)
        tPBSawSquare_free ((tPBSawSquare*)OscModule->theOsc);
    else if (type == OscTypeSineTri)
    {
        tPBSineTriangle_free ((tPBSineTriangle*)OscModule->theOsc);
    }
    else if (type == OscTypeSaw)
    {
        tPBSaw_free ((tPBSaw*)OscModule->theOsc);
    }
    else if (type == OscTypePulse)
    {
        tPBPulse_free ((tPBPulse*)OscModule->theOsc);
    }
    else if (type == OscTypeSine)
    {
        tCycle_free ((tCycle*)OscModule->theOsc);
    }
    else if (type == OscTypeTri)
    {
        tPBTriangle_free ((tPBTriangle*)OscModule->theOsc);
    }
    tExpSmooth_free(&OscModule->pitchSmoother);
    mpool_free((char*)OscModule, OscModule->mempool);
}


// tick function
void tOscModule_tick (tOscModule const osc)
{
    float freqToSmooth = (60 + (osc->fine));
    tExpSmooth_setDest(osc->pitchSmoother, freqToSmooth);
    float tempMIDI =  tExpSmooth_tick(osc->pitchSmoother) + osc->pitchOffset + osc->octaveOffset;

//    float tempIndexF = ((LEAF_clip(-163.0f, tempMIDI, 163.0f) * 100.0f) + 16384.0f);
//    int tempIndexI = (int)tempIndexF;
//    tempIndexF = tempIndexF -tempIndexI;
//    float freqToSmooth1 = osc->mtofTable[tempIndexI & 32767];
//    float freqToSmooth2 = osc->mtofTable[(tempIndexI + 1) & 32767];
    float nowFreq = mtof(tempMIDI);// ((freqToSmooth1 * (1.0f - tempIndexF)) + (freqToSmooth2 * tempIndexF));

    float finalFreq = (nowFreq * osc->harmonicMultiplier ) + osc->freqOffset;
    osc->freq_set_func(osc->theOsc, finalFreq);
    osc->outputs[0] = osc->tick(osc->theOsc) * osc->amp;
}


// Modulatable setters
void tOscModule_setMIDIPitch (tOscModule const osc, float const input)
{

    osc->inputNote = input * 127.0f;

}
void tOscModule_setHarmonic (tOscModule const osc, float harm)
{
    harm -= 0.5f;
    harm *= 2.f;
    harm *= 15.0f;
    if (1)
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

void tOscModule_setPitchOffset (tOscModule const osc, float pitch)
{
    pitch -=0.5f;
    pitch *= 24.0f;
    if (osc->stepped)
    {
        pitch = roundf(pitch);
    }
    osc->pitchOffset = pitch;
}

void tOscModule_setFine (tOscModule const osc, float const fine)
{
    osc->fine = (fine  -0.5f) * 2.f;
}

void tOscModule_setFreq (tOscModule const osc, float const freq)
{

    osc->freqOffset = (freq * 4000.0f) - 2000.f;
}
void tOscModule_setOctave (tOscModule const osc, float const oct)
{
    osc->octaveOffset = (roundf(((oct - 0.5f) * 6.0f))) * 12.0f;
}
void tOscModule_setAmp (tOscModule const osc, float const amp)
{
    osc->amp = amp;
}
void tOscModule_setGlide (tOscModule const osc, float const glide)
{
    float factor = 1.0f;
    if (glide >= 0.00001f) {
        factor = 1.0f - (expf(-osc->invSr / glide));
    }
    tExpSmooth_setFactor(osc->pitchSmoother,factor);
}
void tOscModule_setStepped (tOscModule const osc, float const stepped)
{
    osc->stepped = roundf(stepped);
}

void tOscModule_setSyncMode (tOscModule const osc, float const syncMode)
{
    osc->syncMode = roundf(syncMode);
}

void tOscModule_setSyncIn (tOscModule const osc, float syncIn)
{
    //call the per-type sync functions if they exist
}

// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* const tableAddress)
{
    osc->mtofTable = tableAddress;
}
void tOscModule_setSampleRate (tOscModule const osc, float const sr)
{
    //tCycle_setSampleRate(osc->oscs[0], sr);
}

void tOscModule_processorInit(tOscModule const osc, leaf::tProcessor* const processor)
{
    // Checks that arguments are valid
	if (osc == NULL)
	{
		return;
	}
	if (processor == NULL)
	{
		return;
	}


    processor->processorUniqueID = osc->uniqueID;
    processor->object = osc;
    processor->numSetterFunctions = OscNumParams;
    processor->tick = (tTickFuncReturningVoid )&tOscModule_tick;
    processor->setterFunctions[OscMidiPitch] =(tSetter) &tOscModule_setMIDIPitch;
    processor->setterFunctions[OscHarmonic] = (tSetter)&tOscModule_setHarmonic;
    processor->setterFunctions[OscPitchOffset] = (tSetter)&tOscModule_setPitchOffset;
    processor->setterFunctions[OscPitchFine] = (tSetter)&tOscModule_setFine;
    processor->setterFunctions[OscFreqOffset] =(tSetter) &tOscModule_setFreq;
    processor->setterFunctions[OscShapeParam] = osc->setterFunctions[OscShapeParam];
    processor->setterFunctions[OscAmpParam] = (tSetter)&tOscModule_setAmp;
    processor->setterFunctions[OscGlide] = (tSetter)&tOscModule_setGlide;
    processor->setterFunctions[OscStepped] = (tSetter)&tOscModule_setStepped;
    processor->setterFunctions[OscSyncMode] = (tSetter)&tOscModule_setSyncMode;
    processor->setterFunctions[OscSyncIn] = (tSetter)&tOscModule_setSyncIn;
    processor->setterFunctions[OscType] = (tSetter)&tOscModule_blankFunction;
    osc->setterFunctions[OscMidiPitch] =(tSetter) &tOscModule_setMIDIPitch;
    osc->setterFunctions[OscHarmonic] = (tSetter)&tOscModule_setHarmonic;
    osc->setterFunctions[OscPitchOffset] = (tSetter)&tOscModule_setPitchOffset;
    osc->setterFunctions[OscPitchFine] = (tSetter)&tOscModule_setFine;
    osc->setterFunctions[OscFreqOffset] =(tSetter) &tOscModule_setFreq;
    //osc->setterFunctions[OscShapeParam] = osc->setterFunctions[OscShapeParam];
    osc->setterFunctions[OscAmpParam] = (tSetter)&tOscModule_setAmp;
    osc->setterFunctions[OscGlide] = (tSetter)&tOscModule_setGlide;
    osc->setterFunctions[OscStepped] = (tSetter)&tOscModule_setStepped;
    osc->setterFunctions[OscSyncMode] = (tSetter)&tOscModule_setSyncMode;
    osc->setterFunctions[OscSyncIn] = (tSetter)&tOscModule_setSyncIn;
    osc->setterFunctions[OscType] = (tSetter)&tOscModule_blankFunction;
//    for (int i = 0; i < OscNumParams; i++)
//    {
//        processor->setterFunctions[i](osc, osc->params[i]);
//    }
    processor->inParameters = osc->params;
    processor->outParameters = osc->outputs;
    processor->processorTypeID = ModuleTypeOscModule;
}



