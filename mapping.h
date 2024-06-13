/**********************************************************************/
/*                           Mapping header                           */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef MAPPING_HEADER
#define MAPPING_HEADER

#include <stdio.h>
#include "processor.h"
#include "defs.h"

typedef struct Mapping {
    tSetter setter; // Setter function for the parameter of the mapping
    u_int8_t paramID; //ID for the mapping's parameter
    u_int8_t processorID; // ID for processor that param is in
    void *destObject; // OUT destination for the mapping

    u_int8_t numUsedSources; // Number of active sources for the mapping
    float* inSources[MAX_NUM_SOURCES]; // IN sources from processors
    float scalingValues[MAX_NUM_SOURCES]; // Scaling for the IN sources
    float initialVal; // The mapping's initial value
} tMapping;

void processMapping (tMapping* mapping);

void tMappingInitializer(tMapping *mapping, tProcessor *outputProcessor,
    tProcessor *destProcessor, u_int8_t destParam, u_int8_t source,
    float const scalingValues[MAX_NUM_SOURCES]);

#endif