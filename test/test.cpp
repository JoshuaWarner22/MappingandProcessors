#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2/benchmark/catch_benchmark_all.hpp"

#include <catch2/catch_test_macros.hpp>


#include "processor.h"
#include "../modules/EnvModule.h"
#include "../modules/LFOModule.h"
//#include "../modules/MultiOscModule.h"
#include <random>

#include "../modules/SimpleOscModule.h"
#include "../modules/FilterModule.h"
#define MTOF_TABLE_SIZE	32768
#define MTOF_TABLE_SIZE_MINUS_ONE 32767
#define MTOF_TABLE_SIZE_DIV_TWO	16384

#define SCALE_TABLE_SIZE 2048

// char leafMemory[65535];
//
// float mtofTable[MTOF_TABLE_SIZE];
// float lfoRateTable[SCALE_TABLE_SIZE];
// float myrand()
// {return (float)rand()/RAND_MAX;}
// Define some trivial functions
int func0() { return 0; }
int func1() { return 1; }
int func2() { return 2; }
int func3() { return 3; }
int func4() { return 4; }
int func5() { return 5; }
int func6() { return 6; }
int func7() { return 7; }
int func8() { return 8; }
int func9() { return 9; }

// Array of function pointers
int (*func_ptrs[10])() = {
    func0, func1, func2, func3, func4,
    func5, func6, func7, func8, func9
};

// Switch dispatch
int switch_dispatch(int i) {
    switch (i) {
    case 0: return func0();
    case 1: return func1();
    case 2: return func2();
    case 3: return func3();
    case 4: return func4();
    case 5: return func5();
    case 6: return func6();
    case 7: return func7();
    case 8: return func8();
    case 9: return func9();
    default: return -1;
    }
}

TEST_CASE("Dispatch benchmarks", "[benchmark]")
{
    BENCHMARK_ADVANCED("Function pointer dispatch")
    (Catch::Benchmark::Chronometer meter)
    {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += func_ptrs[i %10]();
            return dummy; // prevent optimization
        });
    };

    BENCHMARK_ADVANCED("Switch statement dispatch")
    (Catch::Benchmark::Chronometer meter)
    {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += switch_dispatch(i % 10);
            return dummy;
        });
    };
}


// Generate 100 trivial functions
template <int N>
int func() {
    return N;
}

// Helper to create an array of function pointers at compile time
template <size_t... Is>
constexpr auto make_func_ptr_array(std::index_sequence<Is...>) {
    return std::array<int(*)(), sizeof...(Is)>{ &func<Is>... };
}

constexpr size_t numFuncs = 100;
constexpr auto funcptrs = make_func_ptr_array(std::make_index_sequence<numFuncs>{});

// Switch dispatch for 100 cases
int switchdispatch(int i) {
    switch (i) {
#define CASE_FUNC(N) case N: return func<N>();
        CASE_FUNC(0)  CASE_FUNC(1)  CASE_FUNC(2)  CASE_FUNC(3)  CASE_FUNC(4)
        CASE_FUNC(5)  CASE_FUNC(6)  CASE_FUNC(7)  CASE_FUNC(8)  CASE_FUNC(9)
        CASE_FUNC(10) CASE_FUNC(11) CASE_FUNC(12) CASE_FUNC(13) CASE_FUNC(14)
        CASE_FUNC(15) CASE_FUNC(16) CASE_FUNC(17) CASE_FUNC(18) CASE_FUNC(19)
        CASE_FUNC(20) CASE_FUNC(21) CASE_FUNC(22) CASE_FUNC(23) CASE_FUNC(24)
        CASE_FUNC(25) CASE_FUNC(26) CASE_FUNC(27) CASE_FUNC(28) CASE_FUNC(29)
        CASE_FUNC(30) CASE_FUNC(31) CASE_FUNC(32) CASE_FUNC(33) CASE_FUNC(34)
        CASE_FUNC(35) CASE_FUNC(36) CASE_FUNC(37) CASE_FUNC(38) CASE_FUNC(39)
        CASE_FUNC(40) CASE_FUNC(41) CASE_FUNC(42) CASE_FUNC(43) CASE_FUNC(44)
        CASE_FUNC(45) CASE_FUNC(46) CASE_FUNC(47) CASE_FUNC(48) CASE_FUNC(49)
        CASE_FUNC(50) CASE_FUNC(51) CASE_FUNC(52) CASE_FUNC(53) CASE_FUNC(54)
        CASE_FUNC(55) CASE_FUNC(56) CASE_FUNC(57) CASE_FUNC(58) CASE_FUNC(59)
        CASE_FUNC(60) CASE_FUNC(61) CASE_FUNC(62) CASE_FUNC(63) CASE_FUNC(64)
        CASE_FUNC(65) CASE_FUNC(66) CASE_FUNC(67) CASE_FUNC(68) CASE_FUNC(69)
        CASE_FUNC(70) CASE_FUNC(71) CASE_FUNC(72) CASE_FUNC(73) CASE_FUNC(74)
        CASE_FUNC(75) CASE_FUNC(76) CASE_FUNC(77) CASE_FUNC(78) CASE_FUNC(79)
        CASE_FUNC(80) CASE_FUNC(81) CASE_FUNC(82) CASE_FUNC(83) CASE_FUNC(84)
        CASE_FUNC(85) CASE_FUNC(86) CASE_FUNC(87) CASE_FUNC(88) CASE_FUNC(89)
        CASE_FUNC(90) CASE_FUNC(91) CASE_FUNC(92) CASE_FUNC(93) CASE_FUNC(94)
        CASE_FUNC(95) CASE_FUNC(96) CASE_FUNC(97) CASE_FUNC(98) CASE_FUNC(99)
#undef CASE_FUNC
    default: return -1;
    }
}

TEST_CASE("Dispatch benchmarks 100 funs", "[benchmark]")
{
    BENCHMARK_ADVANCED("Function pointer dispatch (100 funcs)")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += funcptrs[i % numFuncs]();
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Switch statement dispatch (100 funcs)")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += switchdispatch(i % numFuncs);
            return dummy;
        });
    };
}

// 100 trivial functions named _func0 ... _func99
int _func0()  { return 0; }   int _func1()  { return 1; }   int _func2()  { return 2; }   int _func3()  { return 3; }
int _func4()  { return 4; }   int _func5()  { return 5; }   int _func6()  { return 6; }   int _func7()  { return 7; }
int _func8()  { return 8; }   int _func9()  { return 9; }   int _func10() { return 10; }  int _func11() { return 11; }
int _func12() { return 12; }  int _func13() { return 13; }  int _func14() { return 14; }  int _func15() { return 15; }
int _func16() { return 16; }  int _func17() { return 17; }  int _func18() { return 18; }  int _func19() { return 19; }
int _func20() { return 20; }  int _func21() { return 21; }  int _func22() { return 22; }  int _func23() { return 23; }
int _func24() { return 24; }  int _func25() { return 25; }  int _func26() { return 26; }  int _func27() { return 27; }
int _func28() { return 28; }  int _func29() { return 29; }  int _func30() { return 30; }  int _func31() { return 31; }
int _func32() { return 32; }  int _func33() { return 33; }  int _func34() { return 34; }  int _func35() { return 35; }
int _func36() { return 36; }  int _func37() { return 37; }  int _func38() { return 38; }  int _func39() { return 39; }
int _func40() { return 40; }  int _func41() { return 41; }  int _func42() { return 42; }  int _func43() { return 43; }
int _func44() { return 44; }  int _func45() { return 45; }  int _func46() { return 46; }  int _func47() { return 47; }
int _func48() { return 48; }  int _func49() { return 49; }  int _func50() { return 50; }  int _func51() { return 51; }
int _func52() { return 52; }  int _func53() { return 53; }  int _func54() { return 54; }  int _func55() { return 55; }
int _func56() { return 56; }  int _func57() { return 57; }  int _func58() { return 58; }  int _func59() { return 59; }
int _func60() { return 60; }  int _func61() { return 61; }  int _func62() { return 62; }  int _func63() { return 63; }
int _func64() { return 64; }  int _func65() { return 65; }  int _func66() { return 66; }  int _func67() { return 67; }
int _func68() { return 68; }  int _func69() { return 69; }  int _func70() { return 70; }  int _func71() { return 71; }
int _func72() { return 72; }  int _func73() { return 73; }  int _func74() { return 74; }  int _func75() { return 75; }
int _func76() { return 76; }  int _func77() { return 77; }  int _func78() { return 78; }  int _func79() { return 79; }
int _func80() { return 80; }  int _func81() { return 81; }  int _func82() { return 82; }  int _func83() { return 83; }
int _func84() { return 84; }  int _func85() { return 85; }  int _func86() { return 86; }  int _func87() { return 87; }
int _func88() { return 88; }  int _func89() { return 89; }  int _func90() { return 90; }  int _func91() { return 91; }
int _func92() { return 92; }  int _func93() { return 93; }  int _func94() { return 94; }  int _func95() { return 95; }
int _func96() { return 96; }  int _func97() { return 97; }  int _func98() { return 98; }  int _func99() { return 99; }

// Array of function pointers
int (*funcptrs_[100])() = {
    _func0,  _func1,  _func2,  _func3,  _func4,  _func5,  _func6,  _func7,  _func8,  _func9,
    _func10, _func11, _func12, _func13, _func14, _func15, _func16, _func17, _func18, _func19,
    _func20, _func21, _func22, _func23, _func24, _func25, _func26, _func27, _func28, _func29,
    _func30, _func31, _func32, _func33, _func34, _func35, _func36, _func37, _func38, _func39,
    _func40, _func41, _func42, _func43, _func44, _func45, _func46, _func47, _func48, _func49,
    _func50, _func51, _func52, _func53, _func54, _func55, _func56, _func57, _func58, _func59,
    _func60, _func61, _func62, _func63, _func64, _func65, _func66, _func67, _func68, _func69,
    _func70, _func71, _func72, _func73, _func74, _func75, _func76, _func77, _func78, _func79,
    _func80, _func81, _func82, _func83, _func84, _func85, _func86, _func87, _func88, _func89,
    _func90, _func91, _func92, _func93, _func94, _func95, _func96, _func97, _func98, _func99
};

// Switch dispatch with 100 cases
int switchdispatch_(int i) {
    switch (i) {
    case 0:  return _func0();
    case 1:  return _func1();
    case 2:  return _func2();
    case 3:  return _func3();
    case 4:  return _func4();
    case 5:  return _func5();
    case 6:  return _func6();
    case 7:  return _func7();
    case 8:  return _func8();
    case 9:  return _func9();
    case 10: return _func10();
    case 11: return _func11();
    case 12: return _func12();
    case 13: return _func13();
    case 14: return _func14();
    case 15: return _func15();
    case 16: return _func16();
    case 17: return _func17();
    case 18: return _func18();
    case 19: return _func19();
    case 20: return _func20();
    case 21: return _func21();
    case 22: return _func22();
    case 23: return _func23();
    case 24: return _func24();
    case 25: return _func25();
    case 26: return _func26();
    case 27: return _func27();
    case 28: return _func28();
    case 29: return _func29();
    case 30: return _func30();
    case 31: return _func31();
    case 32: return _func32();
    case 33: return _func33();
    case 34: return _func34();
    case 35: return _func35();
    case 36: return _func36();
    case 37: return _func37();
    case 38: return _func38();
    case 39: return _func39();
    case 40: return _func40();
    case 41: return _func41();
    case 42: return _func42();
    case 43: return _func43();
    case 44: return _func44();
    case 45: return _func45();
    case 46: return _func46();
    case 47: return _func47();
    case 48: return _func48();
    case 49: return _func49();
    case 50: return _func50();
    case 51: return _func51();
    case 52: return _func52();
    case 53: return _func53();
    case 54: return _func54();
    case 55: return _func55();
    case 56: return _func56();
    case 57: return _func57();
    case 58: return _func58();
    case 59: return _func59();
    case 60: return _func60();
    case 61: return _func61();
    case 62: return _func62();
    case 63: return _func63();
    case 64: return _func64();
    case 65: return _func65();
    case 66: return _func66();
    case 67: return _func67();
    case 68: return _func68();
    case 69: return _func69();
    case 70: return _func70();
    case 71: return _func71();
    case 72: return _func72();
    case 73: return _func73();
    case 74: return _func74();
    case 75: return _func75();
    case 76: return _func76();
    case 77: return _func77();
    case 78: return _func78();
    case 79: return _func79();
    case 80: return _func80();
    case 81: return _func81();
    case 82: return _func82();
    case 83: return _func83();
    case 84: return _func84();
    case 85: return _func85();
    case 86: return _func86();
    case 87: return _func87();
    case 88: return _func88();
    case 89: return _func89();
    case 90: return _func90();
    case 91: return _func91();
    case 92: return _func92();
    case 93: return _func93();
    case 94: return _func94();
    case 95: return _func95();
    case 96: return _func96();
    case 97: return _func97();
    case 98: return _func98();
    case 99: return _func99();
    default: return -1;
    }
}
// Generate a fixed-size random index lookup table
static std::array<int, 1000> random_indices = [] {
    std::array<int, 1000> indices{};
    std::mt19937 rng(12345); // fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(0, numFuncs - 1);
    for (auto& x : indices)
        x = dist(rng);
    return indices;
}();
TEST_CASE("Dispatch benchmarks 100 funcs", "[benchmark]") {
    BENCHMARK_ADVANCED("Function pointer dispatch (100 funcs)")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {

            dummy += funcptrs_[i % 100]();
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Switch statement dispatch (100 funcs)")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += switchdispatch_(i % 100);
            return dummy;
        });
    };
}
TEST_CASE("Dispatch benchmarks 100 func rand s", "[benchmark]") {
    BENCHMARK_ADVANCED("Function pointer dispatch (100 funcs)")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            int idx = random_indices[i % random_indices.size()];
            dummy += funcptrs_[idx]();
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Switch statement dispatch (100 funcs)")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            int idx = random_indices[i % random_indices.size()];
            dummy += switchdispatch_(idx);
            return dummy;
        });
    };
}



// --- Virtual dispatch ---
// Base class
struct Base
{
    virtual int call() const = 0;
    virtual ~Base() = default;
};

// Derived classes
template <int N>
struct Derived : Base
{
    int call() const override { return N; }
};

// Create an array of unique_ptr<Base> for all Derived<N>
auto make_virtual_objects()
{
    std::array<std::unique_ptr<Base>, numFuncs> arr{};
    for (size_t i = 0; i < numFuncs; ++i)
    {
        // Use a helper lambda to generate the correct derived type:
        switch (i)
        {
#define CREATE_CASE(N) case N: arr[i] = std::make_unique<Derived<N>>(); break;
            CREATE_CASE(0)  CREATE_CASE(1)  CREATE_CASE(2)  CREATE_CASE(3)  CREATE_CASE(4)
            CREATE_CASE(5)  CREATE_CASE(6)  CREATE_CASE(7)  CREATE_CASE(8)  CREATE_CASE(9)
            CREATE_CASE(10) CREATE_CASE(11) CREATE_CASE(12) CREATE_CASE(13) CREATE_CASE(14)
            CREATE_CASE(15) CREATE_CASE(16) CREATE_CASE(17) CREATE_CASE(18) CREATE_CASE(19)
            CREATE_CASE(20) CREATE_CASE(21) CREATE_CASE(22) CREATE_CASE(23) CREATE_CASE(24)
            CREATE_CASE(25) CREATE_CASE(26) CREATE_CASE(27) CREATE_CASE(28) CREATE_CASE(29)
            CREATE_CASE(30) CREATE_CASE(31) CREATE_CASE(32) CREATE_CASE(33) CREATE_CASE(34)
            CREATE_CASE(35) CREATE_CASE(36) CREATE_CASE(37) CREATE_CASE(38) CREATE_CASE(39)
            CREATE_CASE(40) CREATE_CASE(41) CREATE_CASE(42) CREATE_CASE(43) CREATE_CASE(44)
            CREATE_CASE(45) CREATE_CASE(46) CREATE_CASE(47) CREATE_CASE(48) CREATE_CASE(49)
            CREATE_CASE(50) CREATE_CASE(51) CREATE_CASE(52) CREATE_CASE(53) CREATE_CASE(54)
            CREATE_CASE(55) CREATE_CASE(56) CREATE_CASE(57) CREATE_CASE(58) CREATE_CASE(59)
            CREATE_CASE(60) CREATE_CASE(61) CREATE_CASE(62) CREATE_CASE(63) CREATE_CASE(64)
            CREATE_CASE(65) CREATE_CASE(66) CREATE_CASE(67) CREATE_CASE(68) CREATE_CASE(69)
            CREATE_CASE(70) CREATE_CASE(71) CREATE_CASE(72) CREATE_CASE(73) CREATE_CASE(74)
            CREATE_CASE(75) CREATE_CASE(76) CREATE_CASE(77) CREATE_CASE(78) CREATE_CASE(79)
            CREATE_CASE(80) CREATE_CASE(81) CREATE_CASE(82) CREATE_CASE(83) CREATE_CASE(84)
            CREATE_CASE(85) CREATE_CASE(86) CREATE_CASE(87) CREATE_CASE(88) CREATE_CASE(89)
            CREATE_CASE(90) CREATE_CASE(91) CREATE_CASE(92) CREATE_CASE(93) CREATE_CASE(94)
            CREATE_CASE(95) CREATE_CASE(96) CREATE_CASE(97) CREATE_CASE(98) CREATE_CASE(99)
#undef CREATE_CASE
        }
    }
    return arr;
}

TEST_CASE("Dispatch benchmarks 100 funcs with virtual dispatch", "[benchmark]") {
    auto virtual_objects = make_virtual_objects();

    BENCHMARK_ADVANCED("Function pointer dispatch")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += funcptrs[i % numFuncs]();
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Switch statement dispatch")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += switchdispatch(i % numFuncs);
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Virtual function dispatch")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += virtual_objects[i % numFuncs]->call();
            return dummy;
        });
    };
}



constexpr size_t _numFuncs = 10;

// --- 10 trivial functions ---
template <int N>
int _func() { return N; }

template <size_t... Is>
constexpr auto _make_func_ptr_array(std::index_sequence<Is...>) {
    return std::array<int(*)(), sizeof...(Is)>{ &func<Is>... };
}
constexpr auto _funcptrs = make_func_ptr_array(std::make_index_sequence<numFuncs>{});

// --- Switch dispatch ---
int _switchdispatch(int i) {
    switch (i) {
#define CASE_FUNC(N) case N: return func<N>();
        CASE_FUNC(0)  CASE_FUNC(1)  CASE_FUNC(2)  CASE_FUNC(3)  CASE_FUNC(4)
        CASE_FUNC(5)  CASE_FUNC(6)  CASE_FUNC(7)  CASE_FUNC(8)  CASE_FUNC(9)
#undef CASE_FUNC
    default: return -1;
    }
}

// --- Virtual dispatch ---
struct _Base
{
    virtual int call() const = 0;
    virtual ~_Base() = default;
};

template <int N>
struct _Derived : Base
{
    int call() const override { return N; }
};

auto _make_virtual_objects()
{
    std::array<std::unique_ptr<Base>, numFuncs> arr{};
    for (size_t i = 0; i < numFuncs; ++i)
    {
        switch (i)
        {
#define CREATE_CASE(N) case N: arr[i] = std::make_unique<Derived<N>>(); break;
            CREATE_CASE(0)  CREATE_CASE(1)  CREATE_CASE(2)  CREATE_CASE(3)  CREATE_CASE(4)
            CREATE_CASE(5)  CREATE_CASE(6)  CREATE_CASE(7)  CREATE_CASE(8)  CREATE_CASE(9)
#undef CREATE_CASE
        }
    }
    return arr;
}

TEST_CASE("Dispatch benchmarks 10 funcs with virtual dispatch", "[benchmark]") {
    auto virtual_objects = _make_virtual_objects();

    BENCHMARK_ADVANCED("Function pointer dispatch")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += funcptrs[i % _numFuncs]();
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Switch statement dispatch")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += switchdispatch(i % _numFuncs);
            return dummy;
        });
    };

    BENCHMARK_ADVANCED("Virtual function dispatch")(Catch::Benchmark::Chronometer meter) {
        int dummy = 0;
        meter.measure([&](int i) {
            dummy += virtual_objects[i % _numFuncs]->call();
            return dummy;
        });
    };
}