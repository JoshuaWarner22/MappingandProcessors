add_library(procUtils processorUtils.c processor.h mapping.h)
target_include_directories(procUtils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
