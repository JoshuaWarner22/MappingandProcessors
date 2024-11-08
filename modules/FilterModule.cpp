//
// Created by Jeffrey Snyder on 7/31/24.
//
//
// Created by Joshua Warner on 6/13/24.
//

#include "FilterModule.h"

#include <assert.h>

void tFiltModule_init(void** const filt, float* params, float id, LEAF* const leaf)
{
    tFiltModule_initToPool(filt, params, id, &leaf->mempool);
}

void tFiltModule_blankFunction (tFiltModule const filt, float freq)
{
    ;
}

float dbToATableLookupFunction(float const in, float const sizeMinusOne, float* const tableAddress)
{
    uint32_t inDBIndex = (uint32_t) in;
    uint32_t inDBIndexPlusOne = inDBIndex + 1;
    if (inDBIndexPlusOne > sizeMinusOne)
    {
        inDBIndexPlusOne = sizeMinusOne;
    }
    float alpha = in - (float)inDBIndex;
    return ((tableAddress[inDBIndex] * (1.0f - alpha)) + (tableAddress[inDBIndexPlusOne] * alpha));
}

void tFiltModule_defaultGainSet (tFiltModule const filt, float gain)
{
    float floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
    filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
}

void tFiltModule_PeakGainSet (tFiltModule const filt, float gain)
{
    float floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
    tVZFilterBell_setGain((tVZFilterBell)filt->theFilt, dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress));
}

void tFiltModule_LSGainSet (tFiltModule const filt, float gain)
{
    float floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f  * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
    tVZFilterLS_setGain((tVZFilterLS)filt->theFilt, dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress));
}

void tFiltModule_HSGainSet (tFiltModule const filt, float gain)
{
    float floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f  * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
    tVZFilterHS_setGain((tVZFilterHS)filt->theFilt, dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress));
}


float resTableLookupFunction (float input, float* resTableAddress, float resTableSizeMinusOne)
{
    input *= resTableSizeMinusOne;
	int inputInt = (int)input;
	float inputFloat = (float)inputInt - input;
	int nextPos = LEAF_clip(0.0f, inputInt + 1.0f, resTableSizeMinusOne);
	return LEAF_clip(0.1f, (resTableAddress[inputInt] * (1.0f - inputFloat)) + (resTableAddress[nextPos] * inputFloat), 10.0f);
}

void tFiltModule_ResSetSVF (tFiltModule const filt, float const res)
{
    tSVF_setQ((tSVF)filt->theFilt,res /*resTableLookupFunction(res, filt->resTableAddress, filt->resTableSizeMinusOne)*/);
}

void tFiltModule_ResSetDiode (tFiltModule const filt, float const res)
{
    tDiodeFilter_setQ((tDiodeFilter)filt->theFilt, resTableLookupFunction(res, filt->resTableAddress, filt->resTableSizeMinusOne));
}

void tFiltModule_ResSetPeak (tFiltModule const filt, float const res)
{
    tVZFilterBell_setBandwidth((tVZFilterBell)filt->theFilt, resTableLookupFunction(res*20.0f, filt->resTableAddress, filt->resTableSizeMinusOne));
}

void tFiltModule_ResSetLS (tFiltModule const filt, float const res)
{
    tVZFilterLS_setResonance((tVZFilterLS)filt->theFilt, resTableLookupFunction(res, filt->resTableAddress, filt->resTableSizeMinusOne));
}

void tFiltModule_ResSetHS (tFiltModule const filt, float const res)
{
    tVZFilterHS_setResonance((tVZFilterHS)filt->theFilt, resTableLookupFunction(res, filt->resTableAddress, filt->resTableSizeMinusOne));
}

void tFiltModule_ResSetBR (tFiltModule const filt, float const res)
{
    tVZFilterBR_setResonance((tVZFilterBR)filt->theFilt, resTableLookupFunction(res, filt->resTableAddress, filt->resTableSizeMinusOne));
}

void tFiltModule_ResSetLadder (tFiltModule const filt, float const res)
{
    tLadderFilter_setQ((tLadderFilter)filt->theFilt, resTableLookupFunction(res, filt->resTableAddress, filt->resTableSizeMinusOne));
}


void tFiltModule_initToPool(void** const filt, float* const params, float id, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tFiltModule* FiltModule =static_cast<_tFiltModule *> ( *filt = (_tFiltModule*) mpool_alloc(sizeof(_tFiltModule), m));
    memcpy(FiltModule->params, params, FiltNumParams*sizeof(float));
    FiltModule->uniqueID = id;

    int type = 0.0f;//roundf(FiltModule->params[FiltType]);
    FiltModule->mempool = m;
    FiltModule->amp = 1.0f;
    if (type == FiltTypeLowpass) {
        tSVF_initToPool((tSVF*)&FiltModule->theFilt, SVFTypeLowpass,10000.0f, 0.5f, mempool);
        FiltModule->freq_set_func = (tFiltInternalParamSetFunc)tSVF_setFreqFast;
        FiltModule->gain_set_func = (tFiltInternalParamSetFunc)tFiltModule_defaultGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetSVF;
        FiltModule->tick =(tTickFuncFloatInReturningFloat) tSVF_tickLP;
    }
    else if (type == FiltTypeHighpass) {
        tSVF_initToPool((tSVF*)&FiltModule->theFilt, SVFTypeHighpass,100.0f, 0.5f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tSVF_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_defaultGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetSVF;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tSVF_tickHP;
    }
    else if (type == FiltTypeBandpass) {
        tSVF_initToPool((tSVF*)&FiltModule->theFilt, SVFTypeBandpass,100.0f, 0.5f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tSVF_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_defaultGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetSVF;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tSVF_tickBP;
    }
    else if (type == FiltTypeDiodeLowpass) {
        tDiodeFilter_initToPool((tDiodeFilter*)&FiltModule->theFilt, 10000.0f, 0.5f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tDiodeFilter_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_defaultGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetDiode;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tDiodeFilter_tickEfficient;
    }
    else if (type == FiltTypePeak) {
        tVZFilterBell_initToPool((tVZFilterBell*)&FiltModule->theFilt, 100.0f, 0.5f, 1.0f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tVZFilterBell_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_PeakGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetPeak;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tVZFilterBell_tick;
    }
    else if (type == FiltTypeLowShelf) {
        tVZFilterLS_initToPool((tVZFilterLS*)&FiltModule->theFilt, 100.0f, 0.5f, 1.0f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tVZFilterLS_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_LSGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetLS;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tVZFilterLS_tick;
    }
    else if (type == FiltTypeHighShelf) {
        tVZFilterHS_initToPool((tVZFilterHS*)&FiltModule->theFilt, 100.0f, 0.5f, 1.0f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tVZFilterHS_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_HSGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetHS;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tVZFilterHS_tick;
    }
    else if (type == FiltTypeNotch) {
        tVZFilterBR_initToPool((tVZFilterBR*)&FiltModule->theFilt, 100.0f, 0.5f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tVZFilterBR_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_defaultGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetBR;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tVZFilterBR_tick;
    }
    else if (type == FiltTypeLadderLowpass) {
        tLadderFilter_initToPool((tLadderFilter*)&FiltModule->theFilt, 100.0f, 0.5f, mempool);
        FiltModule->freq_set_func =(tFiltInternalParamSetFunc) tLadderFilter_setFreqFast;
        FiltModule->gain_set_func =(tFiltInternalParamSetFunc) tFiltModule_defaultGainSet;
        FiltModule->Q_set_func =(tFiltInternalParamSetFunc) tFiltModule_ResSetLadder;
        FiltModule->tick = (tTickFuncFloatInReturningFloat)tLadderFilter_tick;
    }
    FiltModule->moduleType = ModuleTypeFilterModule;
    FiltModule->params[FiltAudioInput] = 0.0f;
}


void tFiltModule_free(void** const filt)
{
    _tFiltModule* FiltModule =(_tFiltModule*) *filt;
    int type = roundf(FiltModule->params[FiltType]);
    if (type == FiltTypeLowpass) {
        tSVF_free((tSVF*)FiltModule->theFilt);
    }
    else if (type == FiltTypeHighpass) {
        tSVF_free((tSVF*)FiltModule->theFilt);
    }
    else if (type == FiltTypeBandpass) {
        tSVF_free((tSVF*)FiltModule->theFilt);
    }
    else if (type == FiltTypeDiodeLowpass) {
        tDiodeFilter_free((tDiodeFilter*)FiltModule->theFilt);
    }
    else if (type == FiltTypePeak) {
        tVZFilterBell_free((tVZFilterBell*)FiltModule->theFilt);
    }
    else if (type == FiltTypeLowShelf) {
        tVZFilterLS_free((tVZFilterLS*)FiltModule->theFilt);
    }
    else if (type == FiltTypeHighShelf) {
        tVZFilterHS_free((tVZFilterHS*)FiltModule->theFilt);
    }
    else if (type == FiltTypeNotch) {
        tVZFilterBR_free((tVZFilterBR*)FiltModule->theFilt);
    }
    else if (type == FiltTypeLadderLowpass) {
        tLadderFilter_free((tLadderFilter*)FiltModule->theFilt);
    }
    mpool_free((char*)FiltModule, FiltModule->mempool);
}


// tick function
void tFiltModule_tick (tFiltModule const filt, float* buffer)
{
    float const cutoff  = filt->cutoffKnob + (filt->inputNote  * filt->params[FiltKeyfollow]); // TODO: should this be cutoffKnob * 137 to allow full range of knob turn to map to maxium freq?)
    filt->freq_set_func(filt->theFilt, cutoff);
    buffer[0] += filt->params[FiltAudioInput];
    buffer[0] = filt->outputs[0] = filt->tick(filt->theFilt,  buffer[0]) * filt->amp;
}

// Modulatable setters



void tFiltModule_setMIDIPitch (tFiltModule const filt, float const input)
{
    filt->inputNote = input * 127.0f;
}

void tFiltModule_setCutoff (tFiltModule const filt, float const input)
{
    filt->cutoffKnob = input * 127.0f;
}

// Non-modulatable setters

void tFiltModule_setDBtoATableLocation (tFiltModule const filt, float* const tableAddress, uint32_t const tableSize)
{
    filt->dbTableAddress = tableAddress;
    filt->dbTableSizeMinusOne = (float)(tableSize - 1);
    filt->dbTableScalar = filt->dbTableSizeMinusOne/(4.0f-0.00001f);
    filt->dbTableOffset = 0.00001f * filt->dbTableScalar;
}

void tFiltModule_setResTableLocation (tFiltModule const filt, float* tableAddress, uint32_t const tableSize)
{
    filt->resTableAddress = tableAddress;
    filt->resTableSizeMinusOne = (float)(tableSize - 1);
}

void tFiltModule_setSampleRate (tFiltModule const filt, float const sr)
{
    //tCycle_setSampleRate(filt->filts[0], sr);
}

//be sure to set the tables before initing the processor
void tFiltModule_processorInit(tFiltModule const filt, leaf::tProcessor* const processor)
{
    // Checks that arguments are valid
	if (filt == NULL)
	{
		return;
	}
	if (processor == NULL)
	{
		return;
	}

    processor->processorUniqueID = filt->uniqueID;
    processor->object = filt;
    processor->numSetterFunctions = FiltNumParams;
    processor->tick = (tTickFuncReturningVoid ) &tFiltModule_tick;
    processor->setterFunctions[FiltMidiPitch] = (tSetter)&tFiltModule_setMIDIPitch;
    processor->setterFunctions[FiltCutoff] = (tSetter)&tFiltModule_setCutoff;
    processor->setterFunctions[FiltGain] = (tSetter)&(*filt->gain_set_func);
    processor->setterFunctions[FiltResonance] = (tSetter)&(*filt->Q_set_func);
    processor->setterFunctions[FiltKeyfollow] = (tSetter)&tFiltModule_blankFunction;
    processor->setterFunctions[FiltType] = (tSetter)&tFiltModule_blankFunction;
    processor->setterFunctions[FiltAudioInput] = (tSetter)&tFiltModule_blankFunction;
//    for (int i = 0; i < FiltNumParams; i++)
//    {
//        processor->setterFunctions[i](filt, filt->params[i]);
//    }
    processor->inParameters = filt->params;
    processor->outParameters = filt->outputs;
    processor->processorTypeID = ModuleTypeFilterModule;
}



