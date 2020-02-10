# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/isis/MOOD2Be/BehaviorTree.CPP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/isis/MOOD2Be/BehaviorTree.CPP

# Include any dependencies generated for this target.
include tools/CMakeFiles/bt3_log_cat.dir/depend.make

# Include the progress variables for this target.
include tools/CMakeFiles/bt3_log_cat.dir/progress.make

# Include the compile flags for this target's objects.
include tools/CMakeFiles/bt3_log_cat.dir/flags.make

tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o: tools/CMakeFiles/bt3_log_cat.dir/flags.make
tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o: tools/bt_log_cat.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/isis/MOOD2Be/BehaviorTree.CPP/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o"
	cd /home/isis/MOOD2Be/BehaviorTree.CPP/tools && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o -c /home/isis/MOOD2Be/BehaviorTree.CPP/tools/bt_log_cat.cpp

tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.i"
	cd /home/isis/MOOD2Be/BehaviorTree.CPP/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/isis/MOOD2Be/BehaviorTree.CPP/tools/bt_log_cat.cpp > CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.i

tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.s"
	cd /home/isis/MOOD2Be/BehaviorTree.CPP/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/isis/MOOD2Be/BehaviorTree.CPP/tools/bt_log_cat.cpp -o CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.s

tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.requires:

.PHONY : tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.requires

tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.provides: tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.requires
	$(MAKE) -f tools/CMakeFiles/bt3_log_cat.dir/build.make tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.provides.build
.PHONY : tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.provides

tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.provides.build: tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o


# Object files for target bt3_log_cat
bt3_log_cat_OBJECTS = \
"CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o"

# External object files for target bt3_log_cat
bt3_log_cat_EXTERNAL_OBJECTS =

bin/bt3_log_cat: tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o
bin/bt3_log_cat: tools/CMakeFiles/bt3_log_cat.dir/build.make
bin/bt3_log_cat: lib/libbehaviortree_cpp_v3.so
bin/bt3_log_cat: tools/CMakeFiles/bt3_log_cat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/isis/MOOD2Be/BehaviorTree.CPP/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/bt3_log_cat"
	cd /home/isis/MOOD2Be/BehaviorTree.CPP/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bt3_log_cat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/CMakeFiles/bt3_log_cat.dir/build: bin/bt3_log_cat

.PHONY : tools/CMakeFiles/bt3_log_cat.dir/build

tools/CMakeFiles/bt3_log_cat.dir/requires: tools/CMakeFiles/bt3_log_cat.dir/bt_log_cat.cpp.o.requires

.PHONY : tools/CMakeFiles/bt3_log_cat.dir/requires

tools/CMakeFiles/bt3_log_cat.dir/clean:
	cd /home/isis/MOOD2Be/BehaviorTree.CPP/tools && $(CMAKE_COMMAND) -P CMakeFiles/bt3_log_cat.dir/cmake_clean.cmake
.PHONY : tools/CMakeFiles/bt3_log_cat.dir/clean

tools/CMakeFiles/bt3_log_cat.dir/depend:
	cd /home/isis/MOOD2Be/BehaviorTree.CPP && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/isis/MOOD2Be/BehaviorTree.CPP /home/isis/MOOD2Be/BehaviorTree.CPP/tools /home/isis/MOOD2Be/BehaviorTree.CPP /home/isis/MOOD2Be/BehaviorTree.CPP/tools /home/isis/MOOD2Be/BehaviorTree.CPP/tools/CMakeFiles/bt3_log_cat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/CMakeFiles/bt3_log_cat.dir/depend

