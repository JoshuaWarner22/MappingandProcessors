//
// Created by Davis Polito on 7/11/24.
//

#ifndef UNTITLED_FUNCMAPS_H
#define UNTITLED_FUNCMAPS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "processor.h"
#include "EnvModule.h"
#include "LFOModule.h"
#include "SimpleOscModule.h"
#include "MultiOscModule.h"

createProcFunc proc_init_map[] = {(createProcFunc)tMultiOscModule_processorInit,
                        (createProcFunc)tLFOModule_processorInit,
                        (createProcFunc)tOscModule_processorInit,
                        (createProcFunc)tEnvModule_processorInit};
createModuleFunc module_init_map[] = {(createModuleFunc)tOscModule_init,
                        (createModuleFunc)tLFOModule_init,
                        (createModuleFunc)tOscModule_init,
                        (createModuleFunc)tEnvModule_init};

#ifdef __cplusplus
}  // extern "C"
#endif
#endif //UNTITLED_FUNCMAPS_H
