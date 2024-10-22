/**********************************************************************/
/*            Processor object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/
#include "processor.h"
#include "funcmaps.h"

leaf::createProcFunc proc_init_map[] = {
    (leaf::createProcFunc)tOscModule_processorInit,
    (leaf::createProcFunc)tOscModule_processorInit,
    (leaf::createProcFunc)tOscModule_processorInit};
leaf::createModuleFunc module_init_map[] = {
    (leaf::createModuleFunc)tOscModule_init,
    (leaf::createModuleFunc)tOscModule_init,
    (leaf::createModuleFunc)tOscModule_init};

void createProcessor(float *params, leaf::tProcessor* proc,LEAF* leaf)
{
    float type = params[0];
    float id = params[1];
    float *module_params = &params[2];
    void* mod;
    module_init_map[(int)type](&mod, module_params, id, leaf);
    proc_init_map[(int)type](mod, proc);
}
