/**********************************************************************/
/*            Processor object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/
#include "processor.h"
#include "funcmaps.h"
#include "sysex_chunks.h"
#include <vector>
//leaf::createProcFunc proc_init_map[] = {
//    (leaf::createProcFunc)tOscModule_processorInit,
//    (leaf::createProcFunc)tOscModule_processorInit,
//    (leaf::createProcFunc)tOscModule_processorInit};
//leaf::createModuleFunc module_init_map[] = {
//    (leaf::createModuleFunc)tOscModule_init,
//    (leaf::createModuleFunc)tOscModule_init,
//    (leaf::createModuleFunc)tOscModule_init};

//void createProcessor(float *params, leaf::tProcessor* proc,LEAF* leaf)
//{
//    float type = params[0];
//    float id = params[1];
//    float *module_params = &params[2];
//    void* mod;
//    module_init_map[(int)type](&mod, module_params, id, leaf);
//    proc_init_map[(int)type](mod, proc);
//}
//

void leaf::processor_to_preset(leaf::tProcessor *proc, leaf::tProcessorPresetUnion * preset)
{
    preset->data.processorTag = BYTETAGS::PROCTAG;
    preset->data.processorTypeID = proc->processorTypeID;
    preset->data.processorUniqueID = proc->processorUniqueID;
    preset->data.index = proc->index;
    preset->data.proc_chain = proc->proc_chain;
    for (int i = 0; i < MAX_NUM_PARAMS; i++)
    {
        preset->data.params[i] = proc->inParameters[i];
    }
}

void leaf::preset_to_processor(leaf::tProcessorPresetUnion * preset, leaf::tProcessor *proc)
{
    proc->processorTypeID = preset->data.processorTypeID;
    proc->processorUniqueID = preset->data.processorUniqueID;
    proc->index = preset->data.index;
    proc->proc_chain = preset->data.proc_chain;
    for (int i = 0; i < MAX_NUM_PARAMS; i++)
    {
        proc->inParameters[i] = preset->data.params[i];
    }
}

void leaf::splitProcessorPreset(const leaf::tProcessorPreset *preset, leaf::tProcessorPreset7Bit *preset7Bit) {
    // Split processorTag (8 bits -> 2 chunks of 7 bits)
    splitUint8To7bit(preset->processorTag, preset7Bit->processorTag);

    // Split processorTypeID (8 bits -> 2 chunks of 7 bits)
    splitUint8To7bit(preset->processorTypeID, preset7Bit->processorTypeID);

    // Split processorUniqueID (8 bits -> 2 chunks of 7 bits)
    splitUint8To7bit(preset->processorUniqueID, preset7Bit->processorUniqueID);

    // Split proc_chain (8 bits -> 2 chunks of 7 bits)
    splitUint8To7bit(preset->proc_chain, preset7Bit->proc_chain);

    // Split index (8 bits -> 2 chunks of 7 bits)
    splitUint8To7bit(preset->index, preset7Bit->index);

    // Split each parameter (float -> 5 chunks of 7 bits)
    for (int i = 0; i < MAX_NUM_PARAMS; i++) {
        splitFloatTo7bit(preset->params[i], preset7Bit->params[i]);
    }
}
void leaf::unsplitProcessorPreset(const leaf::tProcessorPreset7Bit *preset7Bit, leaf::tProcessorPreset *preset) {
    // Reconstruct processorTag (2 chunks -> 8 bits)
    preset->processorTag = unsplitUint8(preset7Bit->processorTag);

    // Reconstruct processorTypeID (2 chunks -> 8 bits)
    preset->processorTypeID = unsplitUint8(preset7Bit->processorTypeID);

    // Reconstruct processorUniqueID (2 chunks -> 8 bits)
    preset->processorUniqueID = unsplitUint8(preset7Bit->processorUniqueID);

    // Reconstruct proc_chain (2 chunks -> 8 bits)
    preset->proc_chain = unsplitUint8(preset7Bit->proc_chain);

    // Reconstruct index (2 chunks -> 8 bits)
    preset->index = unsplitUint8(preset7Bit->index);

    // Reconstruct each parameter (5 chunks -> float)
    for (int i = 0; i < MAX_NUM_PARAMS; i++) {
        preset->params[i] = unsplitFloat(preset7Bit->params[i]);
    }
}