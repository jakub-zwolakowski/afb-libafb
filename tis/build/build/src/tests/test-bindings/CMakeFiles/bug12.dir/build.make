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
include src/tests/test-bindings/CMakeFiles/bug12.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/tests/test-bindings/CMakeFiles/bug12.dir/compiler_depend.make

# Include the progress variables for this target.
include src/tests/test-bindings/CMakeFiles/bug12.dir/progress.make

# Include the compile flags for this target's objects.
include src/tests/test-bindings/CMakeFiles/bug12.dir/flags.make

src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.o: src/tests/test-bindings/CMakeFiles/bug12.dir/flags.make
src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.o: ../../../src/tests/test-bindings/bug.c
src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.o: src/tests/test-bindings/CMakeFiles/bug12.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.o"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.o -MF CMakeFiles/bug12.dir/bug.c.o.d -o CMakeFiles/bug12.dir/bug.c.o -c /home/qba/git/afb-libafb/src/tests/test-bindings/bug.c

src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bug12.dir/bug.c.i"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/src/tests/test-bindings/bug.c > CMakeFiles/bug12.dir/bug.c.i

src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bug12.dir/bug.c.s"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/src/tests/test-bindings/bug.c -o CMakeFiles/bug12.dir/bug.c.s

# Object files for target bug12
bug12_OBJECTS = \
"CMakeFiles/bug12.dir/bug.c.o"

# External object files for target bug12
bug12_EXTERNAL_OBJECTS =

src/tests/test-bindings/libbug12.so: src/tests/test-bindings/CMakeFiles/bug12.dir/bug.c.o
src/tests/test-bindings/libbug12.so: src/tests/test-bindings/CMakeFiles/bug12.dir/build.make
src/tests/test-bindings/libbug12.so: src/libafb/libafb.a
src/tests/test-bindings/libbug12.so: src/tests/test-bindings/CMakeFiles/bug12.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qba/git/afb-libafb/tis/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libbug12.so"
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bug12.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tests/test-bindings/CMakeFiles/bug12.dir/build: src/tests/test-bindings/libbug12.so
.PHONY : src/tests/test-bindings/CMakeFiles/bug12.dir/build

src/tests/test-bindings/CMakeFiles/bug12.dir/clean:
	cd /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings && $(CMAKE_COMMAND) -P CMakeFiles/bug12.dir/cmake_clean.cmake
.PHONY : src/tests/test-bindings/CMakeFiles/bug12.dir/clean

src/tests/test-bindings/CMakeFiles/bug12.dir/depend:
	cd /home/qba/git/afb-libafb/tis/build/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qba/git/afb-libafb /home/qba/git/afb-libafb/src/tests/test-bindings /home/qba/git/afb-libafb/tis/build/build /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings /home/qba/git/afb-libafb/tis/build/build/src/tests/test-bindings/CMakeFiles/bug12.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tests/test-bindings/CMakeFiles/bug12.dir/depend

