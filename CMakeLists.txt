cmake_minimum_required(VERSION 3.28)
project(untitled C)

set(CMAKE_C_STANDARD 11)
#add_library(LEAF INTERFACE)

file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.c")


#target_include_directories(LEAF INTERFACE LEAF/leaf/leaf.h)
add_executable(untitled ${SRC})

add_subdirectory(${CMAKE_SOURCE_DIR}/LEAF/leaf)
target_link_libraries(${PROJECT_NAME} LEAF)
#add_dependencies(${PROJECT_NAME} LEAF)
#target_link_libraries(untitled PRIVATE LEAF)