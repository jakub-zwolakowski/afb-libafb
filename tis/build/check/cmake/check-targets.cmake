# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6...3.18)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget Check::check Check::checkShared)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target Check::check
add_library(Check::check STATIC IMPORTED)

set_target_properties(Check::check PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "/home/qba/git/afb-libafb/tis/build/check/src;/home/qba/git/afb-libafb/tis/build/check/src/.."
  INTERFACE_LINK_LIBRARIES "m;rt"
)

# Create imported target Check::checkShared
add_library(Check::checkShared SHARED IMPORTED)

set_target_properties(Check::checkShared PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "/home/qba/git/afb-libafb/tis/build/check/src;/home/qba/git/afb-libafb/tis/build/check/src/.."
  INTERFACE_LINK_LIBRARIES "m;rt"
)

# Import target "Check::check" for configuration "Debug"
set_property(TARGET Check::check APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Check::check PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "/home/qba/git/afb-libafb/tis/build/check/src/libcheck.a"
  )

# Import target "Check::checkShared" for configuration "Debug"
set_property(TARGET Check::checkShared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Check::checkShared PROPERTIES
  IMPORTED_LOCATION_DEBUG "/home/qba/git/afb-libafb/tis/build/check/src/libcheck.so.0.15.2"
  IMPORTED_SONAME_DEBUG "libcheck.so.0"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
