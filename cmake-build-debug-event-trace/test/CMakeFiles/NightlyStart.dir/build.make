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

# Produce verbose output by default.
VERBOSE = 1

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

# Utility rule file for NightlyStart.

# Include any custom commands dependencies for this target.
include test/CMakeFiles/NightlyStart.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/NightlyStart.dir/progress.make

test/CMakeFiles/NightlyStart:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && "/Applications/CLion 2.app/Contents/bin/cmake/mac/aarch64/bin/ctest" -D NightlyStart

NightlyStart: test/CMakeFiles/NightlyStart
NightlyStart: test/CMakeFiles/NightlyStart.dir/build.make
.PHONY : NightlyStart

# Rule to build all files generated by this target.
test/CMakeFiles/NightlyStart.dir/build: NightlyStart
.PHONY : test/CMakeFiles/NightlyStart.dir/build

test/CMakeFiles/NightlyStart.dir/clean:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test && $(CMAKE_COMMAND) -P CMakeFiles/NightlyStart.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/NightlyStart.dir/clean

test/CMakeFiles/NightlyStart.dir/depend:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dp9443/Documents/Princeton/MappingandProcessors /Users/dp9443/Documents/Princeton/MappingandProcessors/test /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/test/CMakeFiles/NightlyStart.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/CMakeFiles/NightlyStart.dir/depend

