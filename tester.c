/**********************************************************************/
/*                         ElectroSynth Test                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <stdio.h>
#include <assert.h>


#include "LEAF/leaf/leaf.h"
#include "LEAF/leaf/Inc/leaf-mempool.h"
#include "modules/SimpleOscModule.h"
#include "modules/FilterModule.h"
#include "modules/LFOModule.h"
#include "modules/EnvModule.h"
#define MTOF_TABLE_SIZE	32768
#define MTOF_TABLE_SIZE_MINUS_ONE 32767
#define MTOF_TABLE_SIZE_DIV_TWO	16384
#define SCALE_TABLE_SIZE 2048
#define ATODB_TABLE_SIZE 2048
#define DBTOA_TABLE_SIZE 2048
#define EXP_BUFFER_SIZE 2048
#define DECAY_EXP_BUFFER_SIZE 4096

char leafMemory[65535];

float mtofTable[MTOF_TABLE_SIZE];
float lfoRateTable[SCALE_TABLE_SIZE];
float resTable[SCALE_TABLE_SIZE];
float envTimeTable[SCALE_TABLE_SIZE];
float atoDbTable[ATODB_TABLE_SIZE];
float dbtoATable[DBTOA_TABLE_SIZE];
float decayExpBuffer[DECAY_EXP_BUFFER_SIZE];
float myrand()
{return (float)rand()/RAND_MAX;}


int main(void) 
{
    // Constructs two processors which encapsulate two different 
    // `_tAdd` objects

    LEAF leaf;
    LEAF_init(&leaf, 44100.f, leafMemory, 65535, &myrand);
    leaf.clearOnAllocation = 1;
    LEAF_generate_table_skew_non_sym(resTable, 0.01f, 10.0f, 0.5f, SCALE_TABLE_SIZE);
    LEAF_generate_table_skew_non_sym(envTimeTable, 0.0f, 20000.0f, 4000.0f, SCALE_TABLE_SIZE);
    LEAF_generate_table_skew_non_sym(lfoRateTable, 0.0f, 30.0f, 2.0f, SCALE_TABLE_SIZE);
    LEAF_generate_mtof(mtofTable, -163.8375f, 163.8375f,  MTOF_TABLE_SIZE); //mtof table for fast calc
    LEAF_generate_exp(decayExpBuffer, 0.001f, 0.0f, 1.0f, -0.0008f, DECAY_EXP_BUFFER_SIZE); // exponential decay buffer falling from 1 to 0
    LEAF_generate_atodb(atoDbTable, ATODB_TABLE_SIZE, 0.00001f, 4.0f);
    LEAF_generate_dbtoa(dbtoATable, DBTOA_TABLE_SIZE, -90.0f, 50.0f);

//    void* module1;
//
//    void* module2;
//
//    void* module3;
//
//    float params1[5] = {10.0f, 440.0f, 44100.0f, 0.5f, 1.0f};
//    float params2[OscNumParams] = {0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
//    float params3[LFONumParams] = {0.0f, 0.5f, 0.5f, 0.5f, 0.5f};
//
//    tMultiOscModule_init(&module1, params1, 0, &leaf);
//    tOscModule_init(&module2, params2,1,  &leaf);
//    tLFOModule_init(&module3, params3, 2, &leaf);
//
//    tProcessor proc1;
//    tProcessor proc2;
//    tProcessor proc3;
//
//    tMultiOscModule_setPhase(module1, 10.0f);
//    tMultiOscModule_setFreq(module1, 440.f);
//    tMultiOscModule_setSampleRate(module1, 44100.f);
//    tMultiOscModule_setAmp(module1, 0.5f);
//    tMultiOscModule_setHarmonic(module1, 1.f);
//
//
//    tMultiOscModule_processorInit(module1, &proc1);
//    tOscModule_processorInit(module2, &proc2);
//    tLFOModule_processorInit(module3, &proc3);
//
//
//    tLFOModule_setRateTableLocation(module3, lfoRateTable);
//    tOscModule_setMTOFTableLocation(module2, mtofTable);
///*
//    tAdder adder1;
//    tAdder_setA (&adder1, 10);
//    tAdder_setB (&adder1, 20);
//
//    tProcessor proc2;
//
//
//    tAddInitializer(&proc2, &adder1, 2);
//
//*/
//    // Constructs and initialized _tMapping object from the output of
//    // `_adder2` to the field `a` of `_adder1`
//
//    tMapping mapping;
//    tMapping_init(&mapping);
//    float scalingValues[] = {1.0, 1.0, 1.0};
//
//    tMappingAdd(&mapping, &proc2, &proc1, MultiOscFreq, 0,
//    scalingValues);
//
//
//    // Run the process without a mapping
//    proc2.tick(proc2.object);
//    proc1.tick(proc1.object);
//    proc3.tick(proc3.object);
//    for(int i = 0; i < 1000; i++) {
//        processMapping(&mapping);
//
//        proc2.tick(proc2.object);
//        proc1.tick(proc1.object);
//        proc3.tick(proc3.object);
//    }
//
//
//    processMapping(&mapping);
//
//    proc2.tick(proc2.object);
//    proc1.tick(proc1.object);
//    proc3.tick(proc3.object);

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
