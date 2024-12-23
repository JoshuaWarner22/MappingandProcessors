//
// Created by Davis Polito on 7/11/24.
//

#ifndef UNTITLED_FUNCMAPS_H
#define UNTITLED_FUNCMAPS_H
//#include "EnvModule.h"
//#include "LFOModule.h"
#include "FilterModule.h"
#include "SimpleOscModule.h"
//each file that includes this will get its own copy of it. this wastes memory. Should find a singleton style solution
static leaf::createProcFunc proc_init_map[] = {
                        (leaf::createProcFunc)tOscModule_processorInit,
                        (leaf::createProcFunc)tFiltModule_processorInit
                        };
static leaf::createModuleFunc module_init_map[] = {
                        (leaf::createModuleFunc)tOscModule_init,
                        (leaf::createModuleFunc)tFiltModule_init
                        };


#endif //UNTITLED_FUNCMAPS_H
