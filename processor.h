/**********************************************************************/
/*                          Processor header                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef PROCESSOR_HEADER
#define PROCESSOR_HEADER

#include <stdio.h>
#include "defs.h"

// Defines a struct Processor as a _tProcessor object
typedef struct Processor
{
    u_int8_t processorUniqueID; // ID for this processor
    u_int8_t processorTypeID; // ID for this processor

    void *object; // Processor's aliased Leaf object
    tTickFunc tick; // The object's tick function
    u_int8_t numSetterFunctions; // Number of setter functions used by
    // the processor's object
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing
    // pointers to the setter functions for the processor's object


    float inParameters[MAX_NUM_PARAMS]; // IN sources for the processor
    float outParameters[MAX_NUM_PARAMS]; // OUT sources for the processor

} tProcessor;

#endif