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
    uint16_t processorUniqueID; // ID for this processors
    uint8_t processorTypeID; // ID for this processors

    void *object; // Processor's aliased Leaf object
    tTickFuncReturningVoid tick; // The object's tick function
    uint8_t numSetterFunctions; // Number of setter functions used by
    // the processors's object
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing
    // pointers to the setter functions for the processors's object


    std::atomic<float>* inParameters; //  points to parameter array of the actual object
    std::atomic<float>* outParameters; // OUT sources for the processors

} tProcessor;

typedef void (*createProcFunc)(void* const module, leaf::tProcessor* proc);


typedef void (*createModuleFunc)(void** const module, float* const params, float id, LEAF* const leaf);


void createProcessor(float *params, leaf::tProcessor* proc,LEAF* leaf);

//int func();
//#ifdef __cplusplus
//}  // extern "C"
//#endif
}
#endif
