/**********************************************************************/
/*                           Mapping header                           */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef MAPPING_HEADER
#define MAPPING_HEADER


#include <stdio.h>
#include "processor.h"
#include "defs.h"
namespace leaf
{
    typedef struct Mapping
    {
        tSetter setter; // Setter function for the parameter of the mapping
        uint8_t uuid; //ID for the mapping's parameter
        uint8_t destinationProcessorUniqueID; // ID for destination processors that param is in

        void* destObject; // OUT destination for the mapping
        int paramID;
        uint8_t numUsedSources; // Number of active sources for the mapping
        int inUUIDS[MAX_NUM_SOURCES];
        std::atomic<float>* bipolarSum[MAX_NUM_SOURCES];
        std::atomic<float>* inSources[MAX_NUM_SOURCES]; // IN sources from processors
        std::atomic<float> scalingValues[MAX_NUM_SOURCES]; // Scaling for the IN sources
        std::atomic<float>* initialVal; // The mapping's initial value
    } tMapping;
}
    void processMapping (leaf::tMapping* mapping);

    void tMapping_init (leaf::tMapping* mapping, LEAF &leaf);

    std::atomic<float>* tMappingAdd (leaf::tMapping* mapping, leaf::tProcessor* outputProcessor, leaf::tProcessor* destProcessor, uint8_t destParam, uint8_t source, LEAF& leaf);


#endif
