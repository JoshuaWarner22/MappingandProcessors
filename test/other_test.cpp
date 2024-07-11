//#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_test_macros.hpp>


#include "processor.h"
#include "../modules/EnvModule.h"
#include "../modules/LFOModule.h"
#include "../modules/MultiOscModule.h"
#include "../modules/SimpleOscModule.h"
#define MTOF_TABLE_SIZE	32768
#define MTOF_TABLE_SIZE_MINUS_ONE 32767
#define MTOF_TABLE_SIZE_DIV_TWO	16384

#define SCALE_TABLE_SIZE 2048

char leafMemory[65535];

float mtofTable[MTOF_TABLE_SIZE];
float lfoRateTable[SCALE_TABLE_SIZE];
float myrand()
{return (float)rand()/RAND_MAX;}

TEST_CASE("Test aa", "[pssor]") {

    LEAF leaf;
    LEAF_init(&leaf, 44100.f, leafMemory, 65535, &myrand);
    leaf.clearOnAllocation = 1;
    LEAF_generate_table_skew_non_sym(lfoRateTable, 0.0f, 30.0f, 2.0f, SCALE_TABLE_SIZE);
    LEAF_generate_mtof(mtofTable, -163.8375f, 163.8375f,  MTOF_TABLE_SIZE); //mtof table for fast calc


    void* module1;

    void* module2;

    void* module3;

    float params1[MultiOscNumParams + 2] = {ModuleTypeMultiOscModule, 0, 10.0f, 440.0f, 44100.0f, 0.5f, 1.0f};
    float params2[OscNumParams + 2] = {ModuleTypeOscModule, 1, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float params3[LFONumParams + 2] = {ModuleTypeLFOModule, 2, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f};

//    tMultiOscModule_init(&module1, params1, 0, &leaf);
//    tOscModule_init(&module2, params2,1,  &leaf);
//    tLFOModule_init(&module3, params3, 2, &leaf);

    tProcessor proc1;
    tProcessor proc2;
    tProcessor proc3;
    func();
//    createProcessor(params1, &proc1,&leaf);
//    createProcessor(params2, &proc2, &leaf);
//    createProcessor(params3, &proc3, &leaf);
//    REQUIRE(proc1.processorTypeID == ModuleTypeMultiOscModule);
//    REQUIRE(proc2.processorTypeID == ModuleTypeOscModule);
//    REQUIRE(proc3.processorTypeID == ModuleTypeLFOModule);

}



