/**********************************************************************/
/*                          Processor header                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef PROCESSOR_HEADER
#define PROCESSOR_HEADER

#include <stdio.h>
#include "defs.h"
#include "leaf.h"

// Defines a struct Processor as a _tProcessor object
typedef struct Processor
{
    u_int8_t processorUniqueID; // ID for this processors
    u_int8_t processorTypeID; // ID for this processors

    void *object; // Processor's aliased Module object
    tTickFuncReturningVoid tick; // The object's tick function
    u_int8_t numSetterFunctions; // Number of setter functions used by
    // the processors's object
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing
    // pointers to the setter functions for the processors's object


    float* inParameters; //  points to parameter array of the actual object
    float* outParameters; // OUT sources for the processors

} tProcessor;

typedef void (*createProcFunc)(void* const module, tProcessor* proc);


typedef void (*createModuleFunc)(void** const module, float* const params, float id, LEAF* const leaf);

//
//void createProcessor(float *params, tProcessor* proc,LEAF* leaf);

int func();
#endif