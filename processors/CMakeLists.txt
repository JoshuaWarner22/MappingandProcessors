cmake_minimum_required(VERSION 3.28)
project(untitled)

# Include directories for LEAF and modules
include_directories("${CMAKE_SOURCE_DIR}/LEAF/leaf")
include_directories("${CMAKE_SOURCE_DIR}/LEAF/leaf/Inc")
include_directories("${CMAKE_SOURCE_DIR}/modules")

# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Add library procUtils with processor.c and mapping.c
add_library(procUtils
        processor.cpp
        mapping.cpp
)

# Include directories for procUtils
target_include_directories(procUtils PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

# Link library LEAF to procUtils
target_link_libraries(procUtils PRIVATE
        LEAF
)

# Optionally, enable testing
#enable_testing()

# Add subdirectory for tests
#add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")
