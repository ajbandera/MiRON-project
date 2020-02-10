#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BehaviorTree::behaviortree_cpp_v3" for configuration ""
set_property(TARGET BehaviorTree::behaviortree_cpp_v3 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(BehaviorTree::behaviortree_cpp_v3 PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libbehaviortree_cpp_v3.so"
  IMPORTED_SONAME_NOCONFIG "libbehaviortree_cpp_v3.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS BehaviorTree::behaviortree_cpp_v3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_BehaviorTree::behaviortree_cpp_v3 "${_IMPORT_PREFIX}/lib/libbehaviortree_cpp_v3.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
