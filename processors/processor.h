/**********************************************************************/
/*                          Processor header                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef PROCESSOR_HEADER
#define PROCESSOR_HEADER
//#ifdef __cplusplus
//extern "C" {
//#endif
#include <stdio.h>
#include "defs.h"
#include "leaf.h"
#include <atomic>
namespace leaf{
// Defines a struct Processor as a _tProcessor object
typedef struct Processor
{
    uint8_t processorUniqueID; // ID for this processors
    uint8_t processorTypeID; // ID for this processors
    uint8_t proc_chain;
    uint8_t index;
    void *object; // Processor's aliased Leaf object
    tTickFuncReturningVoid tick; // The object's tick function
    uint8_t numSetterFunctions; // Number of setter functions used by
    // the processors's object
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing
    // pointers to the setter functions for the processors's object


    std::atomic<float>* inParameters; //  points to parameter array of the actual object
    std::atomic<float>* outParameters; // OUT sources for the processors

} tProcessor;

typedef struct ProcessorPreset
{
    uint8_t processorTag;
    uint8_t processorTypeID; // ID for this processors
    uint8_t processorUniqueID; // ID for this processors
    uint8_t proc_chain;
    uint8_t index;
    float params[MAX_NUM_PARAMS];

} tProcessorPreset;

typedef union
{
    tProcessorPreset data;
    uint8_t bytes[sizeof(tProcessorPreset)];
} tProcessorPresetUnion;
// Struct for holding the 7-bit chunked version of tProcessorPreset
struct tProcessorPreset7Bit {
        // Chunks for each field
        uint8_t processorTag[2];                // 2 chunks of 7-bits
        uint8_t processorTypeID[2];             // 2 chunks of 7-bits
        uint8_t processorUniqueID[2];           // 3 chunks of 7-bits
        uint8_t proc_chain[2];                  // 2 chunks of 7-bits
        uint8_t index[2];                       // 2 chunks of 7-bits
        uint8_t params[MAX_NUM_PARAMS][5];      // Each float -> 5 chunks of 7-bits
    };
// Function definitions for split and unsplit
void splitProcessorPreset(const tProcessorPreset *preset, tProcessorPreset7Bit *preset7Bit);
void unsplitProcessorPreset(const tProcessorPreset7Bit *preset7Bit, tProcessorPreset *preset);


typedef void (*createProcFunc)(void* const module, leaf::tProcessor* proc);


typedef void (*createModuleFunc)(void** const module, float* const params, float id, LEAF* const leaf);

//
//void createProcessor(float *params, leaf::tProcessor* proc,LEAF* leaf);

void processor_to_preset(leaf::tProcessor *, leaf::tProcessorPresetUnion * );

void preset_to_processor(leaf::tProcessorPresetUnion *, leaf::tProcessor *);

//bool areProcessorsEqual(const tProcessor& processor1, const tProcessor& processor2)
//{
//    // Compare all non-pointer members
//    if (processor1.processorUniqueID != processor2.processorUniqueID) return false;
//    if (processor1.processorTypeID != processor2.processorTypeID) return false;
//    if (processor1.proc_chain != processor2.proc_chain) return false;
//    if (processor1.index != processor2.index) return false;
//    if (processor1.numSetterFunctions != processor2.numSetterFunctions) return false;
//
//    // Compare setterFunctions array
//    if (std::memcmp(processor1.setterFunctions, processor2.setterFunctions,
//            sizeof(tSetter) * MAX_NUM_PARAMS) != 0)
//        return false;
//
//    // Compare `inParameters` and `outParameters` (initial values comparison)
//    if (processor1.inParameters != nullptr && processor2.inParameters != nullptr) {
//        // Compare element-wise if they point to something
//        for (size_t i = 0; i < MAX_NUM_PARAMS; ++i) {
//            if (processor1.inParameters[i].load() != processor2.inParameters[i].load()) return false;
//        }
//    } else if (processor1.inParameters != processor2.inParameters) {
//        return false; // One is nullptr while another is not
//    }
//
//    return true; // If all checks passed, processors are equal
//}
//int func(
//#ifdef __cplusplus
//}  // extern "C"
//#endif
}
#endif
