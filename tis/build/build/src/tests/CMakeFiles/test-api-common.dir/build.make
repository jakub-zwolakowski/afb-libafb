# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/qba/git/afb-libafb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qba/git/afb-libafb/tis/build/build

# Include any dependencies generated for this target.
include src/tests/CMakeFiles/test-api-common.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/tests/CMakeFiles/test-api-common.dir/compiler_depend.make

# Include the progress variables for this target.
include src/tests/CMakeFiles/test-api-common.dir/progress.make

# Include the compile flags for this target's objects.
include src/tests/CMakeFiles/test-api-common.dir/flags.make

src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.o: src/tests/CMakeFiles/test-api-common.dir/flags.make
src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.o: ../../../src/tests/test-api-common.c
src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.o: src/tests/CMakeFiles/test-api-common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.o"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.o -MF CMakeFiles/test-api-common.dir/test-api-common.c.o.d -o CMakeFiles/test-api-common.dir/test-api-common.c.o -c /home/qba/git/afb-libafb/src/tests/test-api-common.c

src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test-api-common.dir/test-api-common.c.i"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/src/tests/test-api-common.c > CMakeFiles/test-api-common.dir/test-api-common.c.i

src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test-api-common.dir/test-api-common.c.s"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/src/tests/test-api-common.c -o CMakeFiles/test-api-common.dir/test-api-common.c.s

# Object files for target test-api-common
test__api__common_OBJECTS = \
"CMakeFiles/test-api-common.dir/test-api-common.c.o"

# External object files for target test-api-common
test__api__common_EXTERNAL_OBJECTS =

src/tests/test-api-common: src/tests/CMakeFiles/test-api-common.dir/test-api-common.c.o
src/tests/test-api-common: src/tests/CMakeFiles/test-api-common.dir/build.make
src/tests/test-api-common: src/libafb/libafb.a
src/tests/test-api-common: src/tests/CMakeFiles/test-api-common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qba/git/afb-libafb/tis/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test-api-common"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-api-common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tests/CMakeFiles/test-api-common.dir/build: src/tests/test-api-common
.PHONY : src/tests/CMakeFiles/test-api-common.dir/build

src/tests/CMakeFiles/test-api-common.dir/clean:
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests && $(CMAKE_COMMAND) -P CMakeFiles/test-api-common.dir/cmake_clean.cmake
.PHONY : src/tests/CMakeFiles/test-api-common.dir/clean

src/tests/CMakeFiles/test-api-common.dir/depend:
	cd /home/qba/git/afb-libafb/tis/build/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qba/git/afb-libafb /home/qba/git/afb-libafb/src/tests /home/qba/git/afb-libafb/tis/build/build /home/qba/git/afb-libafb/tis/build/build/src/tests /home/qba/git/afb-libafb/tis/build/build/src/tests/CMakeFiles/test-api-common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tests/CMakeFiles/test-api-common.dir/depend

