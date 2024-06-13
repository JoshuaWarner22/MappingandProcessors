/**********************************************************************/
/*              Mapping object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <stdio.h>
#include <assert.h>
#include "defs.h"
#include "mapping.h"
#include "processor.h"

// Process mapping function
void processMapping (tMapping* mapping)
{
    float sum = mapping->initialVal; 

    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        sum += *mapping->inSources[i] * mapping->scalingValues[i];
    }

    mapping->setter(mapping->destObject, sum);
}

// Initializes the mapping `mapping` to map from the output of 
// `outputProcessor` to the input of `destProcessor` on the parameter 
// `destParam`.  The mapping scales this value by the factors in
// `scalingValues`.
void tMappingInitializer(tMapping *mapping, tProcessor *outputProcessor,
    tProcessor *destProcessor, u_int8_t destParam, u_int8_t source,
    float const scalingValues[MAX_NUM_SOURCES])
{

    // Checks that arguments are valid
    assert (mapping != NULL);
    assert (outputProcessor != NULL);
    assert (destProcessor != NULL);
    assert (destParam >= 0 && destParam <= MAX_NUM_PARAMS);
    assert (scalingValues != NULL);

    // Updates the _tMapping struct with the given arguments
    mapping->inSources[source] = &outputProcessor->outParameters[0];
    mapping->numUsedSources++;
    
    mapping->scalingValues[0] = scalingValues[0];
    mapping->scalingValues[1] = scalingValues[1]; 
    mapping->scalingValues[2] = scalingValues[2]; 
    
    mapping->initialVal = destProcessor->inParameters[destParam];
    mapping->setter = destProcessor->setterFunctions[destParam];
    mapping->processorUniqueID = destProcessor->processorUniqueID;
    mapping->paramID = destParam; 
    mapping->destObject = destProcessor->object;
}