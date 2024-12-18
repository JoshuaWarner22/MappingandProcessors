/**********************************************************************/
/*              Mapping object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <assert.h>
#include "mapping.h"

// Process mapping function
void processMapping (leaf::tMapping* mapping)
{
    float sum = mapping->initialVal; 

    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        sum += *mapping->inSources[i] * mapping->scalingValues[i];
    }

    mapping->setter(mapping->destObject, sum);
}

//clears out a new mapping
void tMapping_init(leaf::tMapping *mapping, LEAF& leaf)
{
    mapping-> numUsedSources= 0;
    mapping->uuid = getNextUuid(&leaf);
}
// Initializes the mapping `mapping` to map from the output of 
// `outputProcessor` to the input of `destProcessor` on the parameter 
// `destParam`.  The mapping scales this value by the factors in
// `scalingValues`.
void tMappingAdd(leaf::tMapping *mapping, leaf::tProcessor *outputProcessor,
    leaf::tProcessor *destProcessor, uint8_t destParam, uint8_t source,
    float const scalingValues[MAX_NUM_SOURCES])
{

    // Checks that arguments are valid
    if (mapping == NULL)
    {
    	return;
    }
    if  (outputProcessor == NULL)
    {
    	return;
    }
    if  (destProcessor == NULL)
    {
    	return;
    }
    if  (destParam < 0 || destParam > MAX_NUM_PARAMS)
    {
    	return;
    }
    if (scalingValues == NULL)
    {
    	return;
    }

    // Updates the _tMapping struct with the given arguments
    mapping->inSources[source] = &outputProcessor->outParameters[0];
    mapping->numUsedSources++;
    
    mapping->scalingValues[0] = scalingValues[0];
    mapping->scalingValues[1] = scalingValues[1]; 
    mapping->scalingValues[2] = scalingValues[2]; 
    
    mapping->initialVal = destProcessor->inParameters[destParam];
    mapping->setter = destProcessor->setterFunctions[destParam];
    mapping->destinationProcessorUniqueID = destProcessor->processorUniqueID;
    mapping->paramID = destParam; 
    mapping->destObject = destProcessor->object;
}
