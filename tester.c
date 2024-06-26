/**********************************************************************/
/*                         ElectroSynth Test                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <stdio.h>
#include <assert.h>
#include "adder.h"
#include "mapping.h"
#include "processor.h"


int main(void) 
{
    // Constructs two processors which encapsulate two different 
    // `_tAdd` objects

    tAdder adder1;
    tAdder_setA (&adder1, 10);
    tAdder_setB (&adder1, 20);

    tProcessor processor1;

    tAdder adder2;
    adder2.a = 30;
    adder2.b = 40;
    
    tProcessor processor2;

    tAddInitializer(&processor1,&adder1,1);
    tAddInitializer(&processor2, &adder2, 2);



 
    // Constructs and initialized _tMapping object from the output of
    // `_adder2` to the field `a` of `_adder1`

    tMapping mapping;
    float scalingValues[] = {1.0, 1.0, 1.0};

    tMappingInitializer(&mapping, &processor2, &processor1, 0, 0,
    scalingValues);


    // Run the process without a mapping
    processor1.outParameters[0] =  processor1.tick(processor1.object);
    processor2.outParameters[0] =  processor2.tick(processor2.object);

    // Print statements for testing
    printf("\nValues of out processors' out parameters after one tick:\n\n");
    printf("_processor1.outParameters[0] = %f (should be 30)\n",  
    processor1.outParameters[0]);
    printf("_processor2.outParameters[0] = %f (should be 70)\n\n",  
    processor2.outParameters[0]);

    // Run the process with the mapping
    processMapping(&mapping);
    processor1.outParameters[0] =  processor1.tick(processor1.object);
    processor2.outParameters[0] =  processor2.tick(processor2.object);

    // Print statements for testing
    printf("Values of out processors' out parameters after one tick with"
    " mapping: \n\n");
    printf("_processor1.outParameters[0] = %f (should be 100)\n",  
    processor1.outParameters[0]);
    printf("_processor2.outParameters[0] = %f (should be 70)\n\n",  
    processor2.outParameters[0]);
}
