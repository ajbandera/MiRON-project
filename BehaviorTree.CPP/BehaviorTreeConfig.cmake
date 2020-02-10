# Generated by CMake 3.5.1

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget BehaviorTree::behaviortree_cpp_v3)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
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


# Create imported target BehaviorTree::behaviortree_cpp_v3
add_library(BehaviorTree::behaviortree_cpp_v3 SHARED IMPORTED)

set_target_properties(BehaviorTree::behaviortree_cpp_v3 PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "ZMQ_FOUND"
  INTERFACE_INCLUDE_DIRECTORIES "/home/isis/MOOD2Be/BehaviorTree.CPP/3rdparty;/home/isis/MOOD2Be/BehaviorTree.CPP/include"
  INTERFACE_LINK_LIBRARIES "-lpthread;dl;zmq"
)

# Import target "BehaviorTree::behaviortree_cpp_v3" for configuration ""
set_property(TARGET BehaviorTree::behaviortree_cpp_v3 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(BehaviorTree::behaviortree_cpp_v3 PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/home/isis/MOOD2Be/BehaviorTree.CPP/lib/libbehaviortree_cpp_v3.so"
  IMPORTED_SONAME_NOCONFIG "libbehaviortree_cpp_v3.so"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
