# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Applications/CLion 2.app/Contents/bin/cmake/mac/aarch64/bin/cmake"

# The command to remove a file.
RM = "/Applications/CLion 2.app/Contents/bin/cmake/mac/aarch64/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dp9443/Documents/Princeton/MappingandProcessors

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace

# Include any dependencies generated for this target.
include test/CMakeFiles/unit_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/unit_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/unit_tests.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/unit_tests.dir/flags.make

test/CMakeFiles/unit_tests.dir/test_calc.cpp.o: test/CMakeFiles/unit_tests.dir/flags.make
test/CMakeFiles/unit_tests.dir/test_calc.cpp.o: /Users/dp9443/Documents/Princeton/MappingandProcessors/test/test_calc.cpp
test/CMakeFiles/unit_tests.dir/test_calc.cpp.o: test/CMakeFiles/unit_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/unit_tests.dir/test_calc.cpp.o"
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/unit_tests.dir/test_calc.cpp.o -MF CMakeFiles/unit_tests.dir/test_calc.cpp.o.d -o CMakeFiles/unit_tests.dir/test_calc.cpp.o -c /Users/dp9443/Documents/Princeton/MappingandProcessors/test/test_calc.cpp

test/CMakeFiles/unit_tests.dir/test_calc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/unit_tests.dir/test_calc.cpp.i"
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dp9443/Documents/Princeton/MappingandProcessors/test/test_calc.cpp > CMakeFiles/unit_tests.dir/test_calc.cpp.i

test/CMakeFiles/unit_tests.dir/test_calc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/unit_tests.dir/test_calc.cpp.s"
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dp9443/Documents/Princeton/MappingandProcessors/test/test_calc.cpp -o CMakeFiles/unit_tests.dir/test_calc.cpp.s

# Object files for target unit_tests
unit_tests_OBJECTS = \
"CMakeFiles/unit_tests.dir/test_calc.cpp.o"

# External object files for target unit_tests
unit_tests_EXTERNAL_OBJECTS =

test/unit_tests: test/CMakeFiles/unit_tests.dir/test_calc.cpp.o
test/unit_tests: test/CMakeFiles/unit_tests.dir/build.make
test/unit_tests: libaaa.a
test/unit_tests: _deps/catch2-build/src/libCatch2Maind.a
test/unit_tests: _deps/catch2-build/src/libCatch2d.a
test/unit_tests: test/CMakeFiles/unit_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable unit_tests"
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unit_tests.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && "/Applications/CLion 2.app/Contents/bin/cmake/mac/aarch64/bin/cmake" -D TEST_TARGET=unit_tests -D TEST_EXECUTABLE=/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test/unit_tests -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test -D TEST_SPEC= -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_LIST=unit_tests_TESTS -D TEST_REPORTER= -D TEST_OUTPUT_DIR= -D TEST_OUTPUT_PREFIX= -D TEST_OUTPUT_SUFFIX= -D TEST_DL_PATHS= -D CTEST_FILE=/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test/unit_tests_tests-b12d07c.cmake -P /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-src/extras/CatchAddTests.cmake

# Rule to build all files generated by this target.
test/CMakeFiles/unit_tests.dir/build: test/unit_tests
.PHONY : test/CMakeFiles/unit_tests.dir/build

test/CMakeFiles/unit_tests.dir/clean:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && $(CMAKE_COMMAND) -P CMakeFiles/unit_tests.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/unit_tests.dir/clean

test/CMakeFiles/unit_tests.dir/depend:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dp9443/Documents/Princeton/MappingandProcessors /Users/dp9443/Documents/Princeton/MappingandProcessors/test /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test/CMakeFiles/unit_tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/CMakeFiles/unit_tests.dir/depend

