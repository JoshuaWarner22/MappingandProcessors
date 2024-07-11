//
// Created by Jeffrey Snyder on 7/9/24.
//


#include "EnvModule.h"

#include <assert.h>

void tEnvModule_init(void** const env, float* params, float id, LEAF* const leaf)
{
    tEnvModule_initToPool(env, params, id, &leaf->mempool);
}

void tEnvModule_blankFunction (tEnvModule const env, float freq)
{
    ;
}
void tEnvModule_initToPool(void** const env, float* const params, float id, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tEnvModule* EnvModule = *env = (_tEnvModule*) mpool_alloc(sizeof(_tEnvModule), m);
    memcpy(EnvModule->params, params, EnvNumParams);
    EnvModule->mempool = m;
    EnvModule->setterFunctions[EnvNoteOnWatchFlag] = &tEnvModule_onNoteOn;
    EnvModule->uniqueID = id;

    tADSRT_initToPool(&EnvModule->theEnv, EnvModule->params[EnvAttack],EnvModule->params[EnvDecay],EnvModule->params[EnvSustain],EnvModule->params[EnvRelease], EnvModule->expTable, EnvModule->tableSize,mempool);
    EnvModule->tick = tADSRT_tick;

    EnvModule->moduleType = ModuleTypeEnvModule;
}


void tEnvModule_free(void** const env)
{
    _tEnvModule* EnvModule = *env;
    tADSRT_free(&EnvModule->theEnv);
    mpool_free((char*)EnvModule, EnvModule->mempool);
}
//tick function
void tEnvModule_tick (tEnvModule const env)
{
    env->outputs[0] = tADSRT_tick(env->theEnv);
}

//special noteOnFunction
void tEnvModule_onNoteOn(tEnvModule const env, float pitch, float velocity)
{
    //env->setterFunctions[LFOPhase](env->theLFO, env->params[LFOPhase]); //call actual function
}
/*
// Modulatable setters
void tEnvModule_setRate (tEnvModule const env, float rate)
{
    rate *= 2047.0f;
    int inputInt = (int)rate;
    float inputFloat = (float)inputInt - rate;
    int nextPos = LEAF_clip(0, inputInt + 1, 2047);
    float tempRate = (env->rateTable[inputInt] * (1.0f - inputFloat)) + (env->rateTable[nextPos] * inputFloat);
    env->setterFunctions[LFORate](env->theLFO, tempRate);
}

// Non-modulatable setters
void tEnvModule_setRateTableLocation (tEnvModule const env, float* tableAddress)
{
    env->rateTable = tableAddress;
}
void tEnvModule_setSampleRate (tEnvModule const env, float sr)
{
    //how to handle this? if then cases for different types?

}


*/

void tEnvModule_processorInit(tEnvModule const env, tProcessor* processor)
{
    // Checks that arguments are valid
    assert (env != NULL);
    assert (processor != NULL);

    processor->processorUniqueID = env->uniqueID;
    processor->object = env;
    processor->numSetterFunctions = EnvNumParams;
    processor->tick = tEnvModule_tick;
    memcpy(processor->setterFunctions, env->setterFunctions, EnvNumParams);
    //write over the rate setter since it has some scaling
    //processors->setterFunctions[LFORate] = &tEnvModule_setRate;
    processor->inParameters = env->params;
    processor->outParameters = env->outputs;
    processor->processorTypeID = ModuleTypeEnvModule;
}
