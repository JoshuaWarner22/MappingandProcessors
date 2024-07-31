include(AddCatch2)
include_directories(${CMAKE_SOURCE_DIR}/processors)
include_directories(${CMAKE_SOURCE_DIR}/modules)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Add library procUtils with processor.c and mapping.c
add_library(procUtils
        ../processors/processor.c
        ../processors/mapping.c
        ../modules/SimpleOscModule.c
        ../modules/EnvModule.c
        ../modules/LFOModule.c
        ../modules/FilterModule.c
        ../modules/FilterModule.h
)

target_link_libraries(procUtils PRIVATE LEAF)
# Optionally, set include directories for procUtils (if needed)
target_include_directories(procUtils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/modules
        ${CMAKE_CURRENT_SOURCE_DIR}/processors

)

add_executable(
        _tests
        test.cpp
        other_test.cpp
)
target_link_libraries(
        _tests  PRIVATE procUtils LEAF Catch2::Catch2WithMain
)
#target_link_libraries(tests unit_tests)
#target_link_libraries(tests Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(_tests)
