//
// Created by Jeffrey Snyder on 7/9/24.
//

#ifndef ENVMODULE_H
#define ENVMODULE_H

#include "../LEAF/leaf/leaf.h"
#include "../processors/defs.h"
#include "../processors/processor.h"
#include "../LEAF/leaf/Inc/leaf-mempool.h"
#include "../LEAF/leaf/Inc/leaf-envelopes.h"

typedef enum {
    EnvNoteOnWatchFlag,
    EnvAttack,
    EnvDecay,
    EnvSustain,
    EnvRelease,
    EnvLeak,
    EnvShapeAttack,
    EnvShapeRelease,
    EnvFollowVelocity,
    EnvNumParams
} EnvParams;


typedef struct _tEnvModule {
    tADSRT theEnv;
    uint32_t moduleType;
    uint32_t uniqueID;
    float params[EnvNumParams];
    float outputs[1];
    float* expTable;
    uint32_t followVelocity;
    uint32_t tableSize;
    tMempool mempool;
} _tEnvModule;

typedef _tEnvModule* tEnvModule;

//init module
void tEnvModule_init(void** const env, float* const params, float id, LEAF* const leaf);
void tEnvModule_initToPool(void** const env, float* const params, float id, tMempool* const mempool);
void tEnvModule_free(void** const env);

//note on action
void tEnvModule_onNoteOn(tEnvModule const env, float pitch, float velocity);

// Modulatable setters
void tEnvModule_setAttack (tEnvModule const env, float input);

void tEnvModule_setDecay (tEnvModule const env, float input);

void tEnvModule_setRelease (tEnvModule const env, float input);

void tEnvModule_setSustain (tEnvModule const env, float const input);

void tEnvModule_setLeak (tEnvModule const env, float const input);

// Non-modulatable setters
void tEnvModule_setExpTableLocation (tEnvModule const env, float* tableAddress);
void tEnvModule_setSampleRate (tEnvModule const env, float sr);

//init processors
void tEnvModule_processorInit(tEnvModule const env, tProcessor* processor);

#endif //ENVMODULE_H
