cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_C_STANDARD 11)
#add_library(LEAF INTERFACE)

file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.c")

file(GLOB _MODULES CONFIGURE_DEPENDS "modules/*.h" "modules/*.c")
#add_library(LEAF)


#target_include_directories(LEAF INTERFACE LEAF/leaf/leaf.h)
#add_executable(${PROJECT_NAME} ${SRC} ${_MODULES}
#)
add_subdirectory(processors)
add_library(${PROJECT_NAME} ${SRC} ${_MODULES}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${_MODULES})
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

target_link_libraries(${PROJECT_NAME} PRIVATE procUtils LEAF)

add_subdirectory(${CMAKE_SOURCE_DIR}/LEAF/leaf)
#target_link_libraries(${PROJECT_NAME} LEAF)
add_subdirectory("${CMAKE_SOURCE_DIR}/test")
#add_dependencies(${PROJECT_NAME} LEAF)
#target_link_libraries(untitled PRIVATE LEAF)

