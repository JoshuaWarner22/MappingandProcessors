cmake_minimum_required(VERSION 3.28)
project(untitled)

# Include directories for LEAF
include_directories("${CMAKE_SOURCE_DIR}/LEAF/leaf")
include_directories("${CMAKE_SOURCE_DIR}/LEAF/leaf/Inc")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Set C standard
set(CMAKE_C_STANDARD 11)

# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Globbing source files
file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.c")
file(GLOB _MODULES CONFIGURE_DEPENDS "modules/*.h" "modules/*.c")

# Add subdirectories for libraries and processors
add_subdirectory(${CMAKE_SOURCE_DIR}/LEAF/leaf)
add_subdirectory(processors)

# Create main library
add_library(${PROJECT_NAME} ${SRC} ${_MODULES})

# Include directories for the main library
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/modules
)

# Link libraries to the main project
target_link_libraries(${PROJECT_NAME} PUBLIC LEAF)

# Enable testing
enable_testing()

# Add subdirectory for tests
add_subdirectory("${CMAKE_SOURCE_DIR}/test")