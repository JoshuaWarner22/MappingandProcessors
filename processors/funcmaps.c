//
// Created by Davis Polito on 2/13/25.
//
#include "funcmaps.h"
void createProcessor(float *params, tProcessor* proc,LEAF* leaf)
{
    float type = params[0];
    float id = params[1];
    float *module_params = &params[2];
    void* mod;
    module_init_map[(int)type](&mod, module_params, id, leaf);
    proc_init_map[(int)type](mod, proc);
}

