//
// Created by Jeffrey Snyder on 7/9/24.
//


#include "EnvModule.h"

#include <assert.h>

void tEnvModule_init(void** const env, float* params, LEAF* const leaf)
{
    tEnvModule_initToPool(env, params, &leaf->mempool);
}

void tEnvModule_blankFunction (tEnvModule const env, float freq)
{
    ;
}
void tEnvModule_initToPool(void** const env, float* const params, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tEnvModule* EnvModule = *env = (_tEnvModule*) mpool_alloc(sizeof(_tEnvModule), m);
    memcpy(EnvModule->params, params, EnvNumParams);
    EnvModule->mempool = m;

    //EnvModule->setterFunctions[EnvNoteOnWatchFlag] = &tEnvModule_onNoteOn;
    EnvModule->followVelocity = roundf(EnvModule->params[EnvFollowVelocity]);
    tADSRT_initToPool(&EnvModule->theEnv, EnvModule->params[EnvAttack],EnvModule->params[EnvDecay],EnvModule->params[EnvSustain],EnvModule->params[EnvRelease], EnvModule->expTable, EnvModule->tableSize,mempool);
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
void tEnvModule_onNoteOn(tEnvModule const env, float const pitch, float const velocity)
{
    float const velUsed = env->followVelocity?velocity:1.0f;
    tADSRT_on(env->theEnv, velUsed);
}
/*
// Modulatable setters

/*
*    EnvNoteOnWatchFlag,
    EnvAttack,
    EnvDecay,
    EnvSustain,
    EnvRelease,
    EnvLeak,
    EnvShapeAttack,
    EnvShapeRelease,
    EnvFollowVelocity,
 */
void tEnvModule_setAttack (tEnvModule const env, float input)
{
    	//lookup table for env times
	//input = LEAF_clip(0.0f, input, 1.0f);
	//scale to lookup range
	input *= 2047.0f;
	int const inputInt = (int)input;
	float const inputFloat = (float)inputInt - input;
	int const nextPos = LEAF_clip(0, inputInt + 1, 2047);
	float const attack = (env->expTable[inputInt] * (1.0f - inputFloat)) + (env->expTable[nextPos] * inputFloat)  + 0.001f;

    tADSRT_setAttack(env->theEnv, attack);
}

void tEnvModule_setDecay (tEnvModule const env, float input)
{
    //lookup table for env times
    //input = LEAF_clip(0.0f, input, 1.0f);
    //scale to lookup range
    input *= 2047.0f;
    int const inputInt = (int)input;
    float const inputFloat = (float)inputInt - input;
    int const nextPos = LEAF_clip(0, inputInt + 1, 2047);
    float const decay = (env->expTable[inputInt] * (1.0f - inputFloat)) + (env->expTable[nextPos] * inputFloat)  + 0.001f;
    tADSRT_setDecay(env->theEnv, decay);
}

void tEnvModule_setRelease (tEnvModule const env, float input)
{
    //lookup table for env times
    //input = LEAF_clip(0.0f, input, 1.0f);
    //scale to lookup range
    input *= 2047.0f;
    int const inputInt = (int)input;
    float const inputFloat = (float)inputInt - input;
    int const nextPos = LEAF_clip(0, inputInt + 1, 2047);
    float const release = (env->expTable[inputInt] * (1.0f - inputFloat)) + (env->expTable[nextPos] * inputFloat)  + 0.001f;
    tADSRT_setRelease(env->theEnv, release);
}

void tEnvModule_setSustain (tEnvModule const env, float const input)
{
    tADSRT_setSustain(env->theEnv, LEAF_clip(0.0f, input, 1.0f));
}

void tEnvModule_setLeak (tEnvModule const env, float const input)
{
    tADSRT_setLeakFactor(env->theEnv, 0.99995f + 0.00005f*(1.f-input));
}




// Non-modulatable setters
void tEnvModule_setExpTableLocation (tEnvModule const env, float* tableAddress)
{
    env->expTable = tableAddress;
}
void tEnvModule_setSampleRate (tEnvModule const env, float sr)
{
    //how to handle this? if then cases for different types?

}

void tEnvModule_processorInit(tEnvModule const env, tProcessor* processor)
{
    // Checks that arguments are valid
    assert (env != NULL);
    assert (processor != NULL);

    processor->processorUniqueID = env->uniqueID;
    processor->object = env;
    processor->numSetterFunctions = EnvNumParams;
    processor->tick = tEnvModule_tick;

    processor->setterFunctions[EnvAttack] = &tEnvModule_setAttack;
    processor->setterFunctions[EnvDecay] = &tEnvModule_setDecay;
    processor->setterFunctions[EnvSustain] = &tEnvModule_setSustain;
    processor->setterFunctions[EnvRelease] = &tEnvModule_setRelease;
    processor->setterFunctions[EnvLeak] = &tEnvModule_setLeak;
    processor->setterFunctions[EnvShapeAttack] = &tEnvModule_blankFunction;
    processor->setterFunctions[EnvShapeRelease] = &tEnvModule_blankFunction;
    processor->inParameters = env->params;
    processor->outParameters = env->outputs;
    processor->processorTypeID = ModuleTypeEnvModule;
}

