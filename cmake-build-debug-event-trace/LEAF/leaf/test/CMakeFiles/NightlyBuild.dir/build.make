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

# Utility rule file for NightlyBuild.

# Include any custom commands dependencies for this target.
include LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/compiler_depend.make

# Include the progress variables for this target.
include LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/progress.make

LEAF/leaf/test/CMakeFiles/NightlyBuild:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/LEAF/leaf/test && "/Applications/CLion 2.app/Contents/bin/cmake/mac/aarch64/bin/ctest" -D NightlyBuild

NightlyBuild: LEAF/leaf/test/CMakeFiles/NightlyBuild
NightlyBuild: LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/build.make
.PHONY : NightlyBuild

# Rule to build all files generated by this target.
LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/build: NightlyBuild
.PHONY : LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/build

LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/clean:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/LEAF/leaf/test && $(CMAKE_COMMAND) -P CMakeFiles/NightlyBuild.dir/cmake_clean.cmake
.PHONY : LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/clean

LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/depend:
	cd /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dp9443/Documents/Princeton/MappingandProcessors /Users/dp9443/Documents/Princeton/MappingandProcessors/LEAF/leaf/test /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/LEAF/leaf/test /Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : LEAF/leaf/test/CMakeFiles/NightlyBuild.dir/depend

