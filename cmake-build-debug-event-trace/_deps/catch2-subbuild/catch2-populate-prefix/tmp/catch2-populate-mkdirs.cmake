# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-src"
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-build"
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix"
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix/tmp"
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp"
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix/src"
  "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/dp9443/Documents/Princeton/MappingandProcessors/cmake-build-debug-event-trace/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
