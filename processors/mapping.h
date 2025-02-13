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
        uint8_t index;
        void* destObject; // OUT destination for the mapping
        uint8_t paramID;
        uint8_t numUsedSources; // Number of active sources for the mapping
        uint8_t inUUIDS[MAX_NUM_SOURCES];
        std::atomic<float> bipolarOffset[MAX_NUM_SOURCES];
        std::atomic<float>* inSources[MAX_NUM_SOURCES]; // IN sources from processors
        std::atomic<float> scalingValues[MAX_NUM_SOURCES]; // Scaling for the IN sources
        std::atomic<float>* initialVal; // The mapping's initial value
    } tMapping;

    typedef struct MappingPreset
    {
        uint8_t mappingTag;
        uint8_t index;
        uint8_t uuid; // ID for this processors
        uint8_t destinationUUID;
        uint8_t destParamID;
        uint8_t numUsedSources;
        uint8_t inUUIDs[MAX_NUM_SOURCES];
        float bipolarOffset[MAX_NUM_SOURCES];
        float scalingValues[MAX_NUM_SOURCES];

    } tMappingPreset;

    typedef union {
        tMappingPreset data;
        uint8_t bytes[sizeof (tMappingPreset)];
    } tMappingPresetUnion;


// 7-Bit representation of tMappingPreset
    typedef struct tMappingPreset7Bit {
        uint8_t mappingTag[2];                   // 2 chunks of 7 bits
        uint8_t index[2];                        // 2 chunks of 7 bits
        uint8_t uuid[2];                         // 2 chunks of 7 bits
        uint8_t destinationUUID[2];              // 2 chunks of 7 bits
        uint8_t destParamID[2];                  // 2 chunks of 7 bits
        uint8_t numUsedSources[2];               // 2 chunks of 7 bits
        uint8_t inUUIDs[MAX_NUM_SOURCES][2];     // Each UUID -> 2 chunks of 7 bits
        uint8_t bipolarOffset[MAX_NUM_SOURCES][5]; // Each float -> 5 chunks of 7 bits
        uint8_t scalingValues[MAX_NUM_SOURCES][5]; // Each float -> 5 chunks of 7 bits
    } tMappingPreset7Bit;

}
void splitMappingPreset(const leaf::tMappingPreset* preset, leaf::tMappingPreset7Bit* preset7Bit);
void unsplitMappingPreset(const leaf::tMappingPreset7Bit* preset7Bit, leaf::tMappingPreset* preset);
    void mapping_to_preset(leaf::tMapping *mapping, leaf::tMappingPresetUnion * preset);

    void preset_to_mapping(leaf::tMappingPresetUnion preset, leaf::tMapping *mapping);

    void processMapping (leaf::tMapping* mapping);

    void tMapping_init (leaf::tMapping* mapping, LEAF &leaf);

    std::atomic<float>* tMappingAdd (leaf::tMapping* mapping, leaf::tProcessor* outputProcessor, leaf::tProcessor* destProcessor, uint8_t destParam, uint8_t source, LEAF& leaf);



#endif
