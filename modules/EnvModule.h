//
// Created by Jeffrey Snyder on 7/9/24.
//

#ifndef ENVMODULE_H
#define ENVMODULE_H
#ifdef __cplusplus
extern "C" {
#endif
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
    EnvUseVelocity,
    EnvNumParams
} EnvParams;


typedef struct _tEnvModule {
    tADSRT theEnv;
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    float params[EnvNumParams];
    float outputs[1];
    float* envTimeTableAddress;
    float envTimeTableSizeMinusOne;
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
void tEnvModule_setRate (tEnvModule const env, float rate);

// Non-modulatable setters
void tEnvModule_setRateTableLocation (tEnvModule const env, float* tableAddress);
void tEnvModule_setSampleRate (tEnvModule const env, float sr);

//init processors
void tEnvModule_processorInit(tEnvModule const env, tProcessor* processor);


#ifdef __cplusplus
}  // extern "C"
#endif
#endif //ENVMODULE_H
