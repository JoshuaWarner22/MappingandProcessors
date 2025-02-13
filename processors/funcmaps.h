//
// Created by Davis Polito on 7/11/24.
//

#ifndef UNTITLED_FUNCMAPS_H
#define UNTITLED_FUNCMAPS_H
#include "EnvModule.h"
#include "FilterModule.h"
#include "LFOModule.h"
#include "SimpleOscModule.h"
#ifdef __cplusplus
namespace leaf
{
#endif
    //each file that includes this will getits own copy of it. this wastes memory. Should find a singleton style solution
    static createProcFunc proc_init_map[] = {
        (createProcFunc) tOscModule_processorInit,
        (createProcFunc) tFiltModule_processorInit,
        (createProcFunc) tEnvModule_processorInit,
        (createProcFunc) tLFOModule_processorInit
    };
    static createModuleFunc module_init_map[] = {
        (createModuleFunc) tOscModule_init,
        (createModuleFunc) tFiltModule_init,
        (createModuleFunc) tEnvModule_init,
        (createModuleFunc) tLFOModule_init
    };
#ifdef __cplusplus
}
        #endif
#endif //UNTITLED_FUNCMAPS_H
