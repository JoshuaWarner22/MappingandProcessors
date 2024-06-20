/**********************************************************************/
/*                         ElectroSynth Test                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <stdio.h>
#include <assert.h>
#include "adder.h"
#include "mapping.h"
#include "processor.h"
#include "LEAF/leaf/leaf.h"
#include "LEAF/leaf/Inc/leaf-mempool.h"
#include "modules/MultiOscModule.h"
char leafMemory[65535];

float myrand()
{return (float)rand()/RAND_MAX;}


int main(void) 
{
    // Constructs two processors which encapsulate two different 
    // `_tAdd` objects

    LEAF leaf;
    LEAF_init(&leaf, 44100.f, leafMemory, 65535, &myrand);
    leaf.clearOnAllocation;

    tMultiOscModule module1;

    tMultiOscModule_init(&module1, &leaf);

    module1->moduleType = ModuleTypeMultiOscModule;
    tProcessor proc1;


    tMultiOscModule_setPhase(module1, 10.0f);
    tMultiOscModule_setFreq(module1, 440.f);
    tMultiOscModule_setSampleRate(module1, 44100.f);
    tMultiOscModule_setAmp(module1, 0.5f);
    tMultiOscModule_setHarmonic(module1, 1.f);


    tMultiOscModule_processorInit(module1, &proc1);


    tAdder adder1;
    tAdder_setA (&adder1, 10);
    tAdder_setB (&adder1, 20);

    tProcessor proc2;


    tAddInitializer(&proc2, &adder1, 2);


    // Constructs and initialized _tMapping object from the output of
    // `_adder2` to the field `a` of `_adder1`

    tMapping mapping;
    float scalingValues[] = {1.0, 1.0, 1.0};

    tMappingInitializer(&mapping, &proc2, &proc1, MultiOscFreq, 0,
    scalingValues);


    // Run the process without a mapping
    proc2.outParameters[0] =  proc2.tick(proc2.object);
    proc1.outParameters[0] = proc1.tick(proc1.object);

    processMapping(&mapping);

    proc2.outParameters[0] =  proc2.tick(proc2.object);
    proc1.outParameters[0] = proc1.tick(proc1.object);




//
//    // Run the process with the mapping
//    processMapping(&mapping);
//    processor1.outParameters[0] =  processor1.tick(processor1.object);
//    processor2.outParameters[0] =  processor2.tick(processor2.object);
//
//    // Print statements for testing
//    printf("Values of out processors' out parameters after one tick with"
//    " mapping: \n\n");
//    printf("_processor1.outParameters[0] = %f (should be 100)\n",
//    processor1.outParameters[0]);
//    printf("_processor2.outParameters[0] = %f (should be 70)\n\n",
//    processor2.outParameters[0]);
}
