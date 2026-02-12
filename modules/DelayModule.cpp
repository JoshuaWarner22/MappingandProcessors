//
// Created by Myra Norton on 1/22/26.
//

#include "DelayModule.h"
#include <assert.h>

// lagrange and tape delay

void tDelayModule_init(void** const delay, float* params, float id, LEAF* const leaf) {
	if(leaf->resTable == NULL)
		{
		tLookupTable_create(&leaf->mempool,&leaf->resTable);
		tLookupTable_init(leaf, leaf->resTable,  0.0f, 10.0f, 0.5f, 2048);
	}
    tDelayModule_initToPool(delay, params, id, &leaf->mempool, leaf->resTable);
}

// float dbToATableLookupFunction(float const in, float const sizeMinusOne, float* const tableAddress)
// {
//     uint32_t inDBIndex = (uint32_t) in;
//     uint32_t inDBIndexPlusOne = inDBIndex + 1;
//     if (inDBIndexPlusOne > sizeMinusOne)
//     {
//         inDBIndexPlusOne = sizeMinusOne;
//     }
//     float alpha = in - (float)inDBIndex;
//     return ((tableAddress[inDBIndex] * (1.0f - alpha)) + (tableAddress[inDBIndexPlusOne] * alpha));
// }

void tDelayModule_setDelay(tDelayModule const delay, float const time)
{
    tDelay_setDelay((tDelay*)delay->theDelay, time*delay->sr);
    // switch(delay->delayType)
    // {
    //     case DelayTypeDelay:
    //         tDelay_setDelay((tDelay*)delay->theDelay, time);
    //         break;
    //     case DelayTypeLinear:
    //         tLinearDelay_setDelay((tLinearDelay*)delay->theDelay, time);
    //         break;
    //     case DelayTypeHermite:
    //         tHermiteDelay_setDelay((tHermiteDelay*)delay->theDelay, time);
    //         break;
    //     case DelayTypeLagrange:
    //         tLagrangeDelay_setDelay((tLagrangeDelay*)delay->theDelay, time);
    //         break;
    //     case DelayTypeAllpass:
    //         tAllpassDelay_setDelay((tAllpassDelay*)delay->theDelay, time);
    //         break;
    //     case DelayTypeTape:
    //         tTapeDelay_setDelay((tTapeDelay*)delay->theDelay, time);
    //         break;
    //     default:
    //         break;
    // }
}

void tDelayModule_setGain(tDelayModule const delay, float const gain)
{
	float floatIndex = 0.0f;
    delay->amp = gain;
    // floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * delay->dbTableScalar) -  delay->dbTableOffset, delay->dbTableSizeMinusOne);
    tDelay_setGain((tDelay*)delay->theDelay, gain);
	// switch(delay->delayType)
	// {
	//     case DelayTypeDelay:
	//         floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * delay->dbTableScalar) -  delay->dbTableOffset, delay->dbTableSizeMinusOne);
	//         tDelay_setGain((tDelay*)delay->theDelay, dbToATableLookupFunction(floatIndex, delay->dbTableSizeMinusOne, delay->dbTableAddress));
	//         break;
	//     case DelayTypeLinear:
	//         floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * delay->dbTableScalar) -  delay->dbTableOffset, delay->dbTableSizeMinusOne);
	//         tLinearDelay_setGain((tLinearDelay*)delay->theDelay, dbToATableLookupFunction(floatIndex, delay->dbTableSizeMinusOne, delay->dbTableAddress));
	//         break;
	//     case DelayTypeHermite:
	//         floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * delay->dbTableScalar) -  delay->dbTableOffset, delay->dbTableSizeMinusOne);
	//         tHermiteDelay_setGain((tHermiteDelay*)delay->theDelay, dbToATableLookupFunction(floatIndex, delay->dbTableSizeMinusOne, delay->dbTableAddress));
	//         break;
	//     case DelayTypeLagrange:
	//         break;
	//     case DelayTypeAllpass:
	//         floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * delay->dbTableScalar) -  delay->dbTableOffset, delay->dbTableSizeMinusOne);
	//         tAllpassDelay_setGain((tAllpassDelay*)delay->theDelay, dbToATableLookupFunction(floatIndex, delay->dbTableSizeMinusOne, delay->dbTableAddress));
	//         break;
	//     case DelayTypeTape:
	//         floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * delay->dbTableScalar) -  delay->dbTableOffset, delay->dbTableSizeMinusOne);
	//         tTapeDelay_setGain((tTapeDelay*)delay->theDelay, dbToATableLookupFunction(floatIndex, delay->dbTableSizeMinusOne, delay->dbTableAddress));
	//         break;
	//     default:
	//         break;
	// }
}

void tDelayModule_initToPool(void** const delay, float* const params, float id, tMempool** const mempool, tLookupTable* resTable)
{
    tMempool* m = *mempool;
    _tDelayModule* DelayModule =(_tDelayModule *) ( *delay = (_tDelayModule*) mpool_alloc(sizeof(_tDelayModule), m));
#ifndef __cplusplus
    memcpy(DelayModule->params, params, DelayNumParams*sizeof(float));

#endif
    DelayModule->header.uniqueID = id;
    //CPPDEREF DelayModule->params[DelayAudioInput] = 0.0f;
    // int type = roundf(DelayModule->params[DelayType]);
    DelayModule->mempool = m;
    DelayModule->invSr = m->leaf->invSampleRate;
    DelayModule->sr = m->leaf->sampleRate;

    tDelay_create(mempool, (tDelay**)&DelayModule->theDelay);
    tDelay_init(m->leaf,(tDelay*)DelayModule->theDelay, 500.0f, 50000.f);

    // switch(type)
    // {
    //     case DelayTypeDelay:
    //         tDelay_create(mempool, (tDelay**)&DelayModule->theDelay);
    //         tDelay_init(m->leaf,(tDelay*)DelayModule->theDelay, 10000.0f, 0.5f);
    //     case DelayTypeLinear:
    //         tLinearDelay_create(mempool, (tLinearDelay**)&DelayModule->theDelay);
    //         tLinearDelay_init(m->leaf,(tLinearDelay*)DelayModule->theDelay, 10000.0f, 0.5f);
    //     case DelayTypeHermite:
    //         tHermiteDelay_create(mempool, (tHermiteDelay**)&DelayModule->theDelay);
    //         tHermiteDelay_init(m->leaf,(tHermiteDelay*)DelayModule->theDelay, 10000.0f, 0.5f);
    //     case DelayTypeLagrange:
    //         tLagrangeDelay_create(mempool, (tLagrangeDelay**)&DelayModule->theDelay);
    //         tLagrangeDelay_init(m->leaf,(tLagrangeDelay*)DelayModule->theDelay, 10000.0f, 0.5f);
    //     case DelayTypeAllpass:
    //         tAllpassDelay_create(mempool, (tAllpassDelay**)&DelayModule->theDelay);
    //         tAllpassDelay_init(m->leaf,(tAllpassDelay*)DelayModule->theDelay, 10000.0f, 0.5f);
    //     case DelayTypeTape:
    //         tTapeDelay_create(mempool, (tTapeDelay**)&DelayModule->theDelay);
    //         tTapeDelay_init(m->leaf,(tTapeDelay*)DelayModule->theDelay, 10000.0f, 0.5f);
    //     default:
    //         break;
    // }
    DelayModule->header.moduleType = ModuleTypeDelayModule;

#ifndef __cplusplus
    for (int i = 0; i < DelayNumParams; i++)
    {
    	tDelayModule_setParameter(DelayModule, i, DelayModule->params[i]);

    }
#endif

}

void tDelayModule_free(void** const delay)
{
    _tDelayModule* DelayModule =(_tDelayModule*) *delay;
    int type = roundf(CPPDEREF DelayModule->header.params[DelayType]);
    tDelay_free((tDelay**)DelayModule->theDelay);

    // switch(type)
    // {
    //     case DelayTypeDelay:
    //         tDelay_free((tDelay**)DelayModule->theDelay);
    //     case DelayTypeLinear:
    //         tLinearDelay_free((tLinearDelay**)DelayModule->theDelay);
    //     case DelayTypeHermite:
    //         tHermiteDelay_free((tHermiteDelay**)DelayModule->theDelay);
    //     case DelayTypeLagrange:
    //         tLagrangeDelay_free((tLagrangeDelay**)DelayModule->theDelay);
    //     case DelayTypeAllpass:
    //         tAllpassDelay_free((tAllpassDelay**)DelayModule->theDelay);
    //     case DelayTypeTape:
    //         tTapeDelay_free((tTapeDelay**)DelayModule->theDelay);
    //     default:
    //         break;
    // }
    mpool_free((char*)DelayModule, DelayModule->mempool);
}

// tick function
void tDelayModule_tick (tDelayModule const delay, float* buffer)
{
    buffer[0] = delay->header.outputs[0] = tDelay_tick((tDelay*)delay->theDelay,  buffer[0]) * delay->amp + buffer[0];
    // switch(delay->delayType)
    // {
    //     case DelayTypeDelay:
    //         buffer[0] = delay->header.outputs[0] = tDelay_tick((tDelay*)delay->theDelay,  buffer[0]) * delay->amp;
    //         break;
    //     case DelayTypeLinear:
    //         buffer[0] = delay->header.outputs[0] = tLinearDelay_tick((tLinearDelay*)delay->theDelay,  buffer[0]) * delay->amp;
    //         break;
    //     case DelayTypeHermite:
    //         buffer[0] = delay->header.outputs[0] = tHermiteDelay_tick((tHermiteDelay*)delay->theDelay,  buffer[0]) * delay->amp;
    //         break;
    //     case DelayTypeLagrange:
    //         buffer[0] = delay->header.outputs[0] = tLagrangeDelay_tick((tLagrangeDelay*)delay->theDelay,  buffer[0]) * delay->amp;
    //         break;
    //     case DelayTypeAllpass:
    //         buffer[0] = delay->header.outputs[0] = tAllpassDelay_tick((tAllpassDelay*)delay->theDelay,  buffer[0]) * delay->amp;
    //         break;
    //     case DelayTypeTape:
    //         buffer[0] = delay->header.outputs[0] = tTapeDelay_tick((tTapeDelay*)delay->theDelay,  buffer[0]) * delay->amp;
    //         break;
    //     default:
    //         break;
    // }
}

// Modulatable setters

void tDelayModule_setParameter(tDelayModule const delay, DelParams param_type,float input)
{
    switch (param_type) {
        case DelayTime:
            tDelayModule_setDelay(delay, input);
            break;
        case DelayGain:
            tDelayModule_setGain(delay, input);
            break;
        // case DelayFeedback:
        //     delay->feedback = input;
        //     break;
        // case DelayMix:
        //     delay->mix = input;
        //     break;
        default:
            break;
    }
}

// Non-modulatable setters
void tDelayModule_setDBtoATableLocation (tDelayModule const delay, float* const tableAddress, uint32_t const tableSize)
{
    delay->dbTableAddress = tableAddress;
    delay->dbTableSizeMinusOne = (float)(tableSize - 1);
    delay->dbTableScalar = delay->dbTableSizeMinusOne/(4.0f-0.00001f);
    delay->dbTableOffset = 0.00001f * delay->dbTableScalar;
}

void tDelayModule_setSampleRate (tDelayModule const delay, float const sr)
{
    //tCycle_setSampleRate(delay->delays[0], sr);
}