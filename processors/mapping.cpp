/**********************************************************************/
/*              Mapping object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <assert.h>
#include "mapping.h"
#include "sysex_chunks.h"
// Process mapping function
void processMapping (leaf::tMapping* mapping)
{
    float sum = *mapping->initialVal;

    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        sum += (*mapping->inSources[i] * mapping->scalingValues[i]) + mapping->bipolarOffset[i];
    }

    mapping->setter(mapping->destObject, sum);
}

//clears out a new mapping
void tMapping_init(leaf::tMapping *mapping, LEAF& leaf)
{
    mapping->numUsedSources= 0;

    for(int i = 0; i < MAX_NUM_SOURCES; i++)
    {
        mapping->scalingValues[i] = 0;
    }
}
// Initializes the mapping `mapping` to map from the output of 
// `outputProcessor` to the input of `destProcessor` on the parameter 
// `destParam`.  The mapping scales this value by the factors in
// `scalingValues`.
std::atomic<float>* tMappingAdd(leaf::tMapping *mapping, leaf::tProcessor *outputProcessor,
    leaf::tProcessor *destProcessor, uint8_t destParam, uint8_t source, LEAF& leaf)

{


    // Checks that arguments are valid
    if (mapping == NULL)
    {
    	return nullptr;
    }
    if  (outputProcessor == NULL)
    {
        return nullptr;
    }
    if  (destProcessor == NULL)
    {
        return nullptr;
    }
    if  (destParam < 0 || destParam > MAX_NUM_PARAMS)
    {
        return nullptr;
    }
//    if (scalingValues == NULL)
//    {
//    	return;
//    }
    mapping->uuid = getNextUuid(&leaf);
    // Updates the _tMapping struct with the given arguments
    mapping->inSources[source] = &outputProcessor->outParameters[0];
    mapping->inUUIDS[source] = outputProcessor->processorUniqueID;
    mapping->numUsedSources++;
    
//    mapping->scalingValues[0] = scalingValues[0];
//    mapping->scalingValues[1] = scalingValues[1];
//    mapping->scalingValues[2] = scalingValues[2];
    
    mapping->initialVal = &destProcessor->inParameters[destParam];
    mapping->setter = destProcessor->setterFunctions[destParam];
    mapping->destinationProcessorUniqueID = destProcessor->processorUniqueID;
    mapping->paramID = destParam; 
    mapping->destObject = destProcessor->object;
    return &mapping->scalingValues[source];
}

void mapping_to_preset(leaf::tMapping *mapping, leaf::tMappingPresetUnion * preset)
{
    preset->data.mappingTag = BYTETAGS::MAPTAG;//;mapping->uuid;
    preset->data.uuid = mapping->uuid;
    preset->data.destinationUUID = mapping->destinationProcessorUniqueID;
    preset->data.destParamID = mapping->paramID;
    preset->data.numUsedSources = mapping->numUsedSources;
    preset->data.index = mapping->index;
    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        preset->data.inUUIDs[i] = mapping->inUUIDS[i];
        preset->data.bipolarOffset[i] = mapping->bipolarOffset[i];
        preset->data.scalingValues[i] = mapping->scalingValues[i];
    }
}

void preset_to_mapping(leaf::tMappingPresetUnion preset, leaf::tMapping *mapping)
{
    mapping->uuid = preset.data.uuid;
    mapping->destinationProcessorUniqueID = preset.data.destinationUUID;
    mapping->paramID = preset.data.destParamID;
    mapping->numUsedSources = preset.data.numUsedSources;
    mapping->index = preset.data.index;
    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        mapping->inUUIDS[i] = preset.data.inUUIDs[i];
        mapping->bipolarOffset[i] = preset.data.bipolarOffset[i];
        mapping->scalingValues[i] = preset.data.scalingValues[i];
    }
}

void splitMappingPreset(const leaf::tMappingPreset* preset, leaf::tMappingPreset7Bit* preset7Bit) {
    // Split basic fields
    splitUint8To7bit(preset->mappingTag, preset7Bit->mappingTag);
    splitUint8To7bit(preset->index, preset7Bit->index);
    splitUint8To7bit(preset->uuid, preset7Bit->uuid);
    splitUint8To7bit(preset->destinationUUID, preset7Bit->destinationUUID);
    splitUint8To7bit(preset->destParamID, preset7Bit->destParamID);
    splitUint8To7bit(preset->numUsedSources, preset7Bit->numUsedSources);

    // Split inUUIDs
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        splitUint8To7bit(preset->inUUIDs[i], preset7Bit->inUUIDs[i]);
    }

    // Split bipolarOffset
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        splitFloatTo7bit(preset->bipolarOffset[i], preset7Bit->bipolarOffset[i]);
    }

    // Split scalingValues
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        splitFloatTo7bit(preset->scalingValues[i], preset7Bit->scalingValues[i]);
    }
}

void unsplitMappingPreset(const leaf::tMappingPreset7Bit* preset7Bit, leaf::tMappingPreset* preset) {
    // Reconstruct basic fields
    preset->mappingTag = unsplitUint8(preset7Bit->mappingTag);
    preset->index = unsplitUint8(preset7Bit->index);
    preset->uuid = unsplitUint8(preset7Bit->uuid);
    preset->destinationUUID = unsplitUint8(preset7Bit->destinationUUID);
    preset->destParamID = unsplitUint8(preset7Bit->destParamID);
    preset->numUsedSources = unsplitUint8(preset7Bit->numUsedSources);

    // Reconstruct inUUIDs
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        preset->inUUIDs[i] = unsplitUint8(preset7Bit->inUUIDs[i]);
    }

    // Reconstruct bipolarOffset
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        preset->bipolarOffset[i] = unsplitFloat(preset7Bit->bipolarOffset[i]);
    }

    // Reconstruct scalingValues
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        preset->scalingValues[i] = unsplitFloat(preset7Bit->scalingValues[i]);
    }
}