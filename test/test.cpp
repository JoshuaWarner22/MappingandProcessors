//#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_test_macros.hpp>


#include "processor.h"
#include "../modules/EnvModule.h"
#include "../modules/LFOModule.h"
//#include "../modules/MultiOscModule.h"
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

TEST_CASE("Test aa", "[pssor]")
{
    LEAF leaf;
    LEAF_init(&leaf, 44100.f, leafMemory, 65535, &myrand);
    leaf.clearOnAllocation = 1;


}