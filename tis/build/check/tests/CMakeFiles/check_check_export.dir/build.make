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
CMAKE_SOURCE_DIR = /home/qba/git/afb-libafb/check

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qba/git/afb-libafb/tis/build/check

# Include any dependencies generated for this target.
include tests/CMakeFiles/check_check_export.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/check_check_export.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/check_check_export.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/check_check_export.dir/flags.make

tests/CMakeFiles/check_check_export.dir/check_check_sub.c.o: tests/CMakeFiles/check_check_export.dir/flags.make
tests/CMakeFiles/check_check_export.dir/check_check_sub.c.o: /home/qba/git/afb-libafb/check/tests/check_check_sub.c
tests/CMakeFiles/check_check_export.dir/check_check_sub.c.o: tests/CMakeFiles/check_check_export.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/check/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/check_check_export.dir/check_check_sub.c.o"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/check_check_export.dir/check_check_sub.c.o -MF CMakeFiles/check_check_export.dir/check_check_sub.c.o.d -o CMakeFiles/check_check_export.dir/check_check_sub.c.o -c /home/qba/git/afb-libafb/check/tests/check_check_sub.c

tests/CMakeFiles/check_check_export.dir/check_check_sub.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/check_check_export.dir/check_check_sub.c.i"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/check/tests/check_check_sub.c > CMakeFiles/check_check_export.dir/check_check_sub.c.i

tests/CMakeFiles/check_check_export.dir/check_check_sub.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/check_check_export.dir/check_check_sub.c.s"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/check/tests/check_check_sub.c -o CMakeFiles/check_check_export.dir/check_check_sub.c.s

tests/CMakeFiles/check_check_export.dir/check_check_master.c.o: tests/CMakeFiles/check_check_export.dir/flags.make
tests/CMakeFiles/check_check_export.dir/check_check_master.c.o: /home/qba/git/afb-libafb/check/tests/check_check_master.c
tests/CMakeFiles/check_check_export.dir/check_check_master.c.o: tests/CMakeFiles/check_check_export.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/check/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/check_check_export.dir/check_check_master.c.o"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/check_check_export.dir/check_check_master.c.o -MF CMakeFiles/check_check_export.dir/check_check_master.c.o.d -o CMakeFiles/check_check_export.dir/check_check_master.c.o -c /home/qba/git/afb-libafb/check/tests/check_check_master.c

tests/CMakeFiles/check_check_export.dir/check_check_master.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/check_check_export.dir/check_check_master.c.i"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/check/tests/check_check_master.c > CMakeFiles/check_check_export.dir/check_check_master.c.i

tests/CMakeFiles/check_check_export.dir/check_check_master.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/check_check_export.dir/check_check_master.c.s"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/check/tests/check_check_master.c -o CMakeFiles/check_check_export.dir/check_check_master.c.s

tests/CMakeFiles/check_check_export.dir/check_check_log.c.o: tests/CMakeFiles/check_check_export.dir/flags.make
tests/CMakeFiles/check_check_export.dir/check_check_log.c.o: /home/qba/git/afb-libafb/check/tests/check_check_log.c
tests/CMakeFiles/check_check_export.dir/check_check_log.c.o: tests/CMakeFiles/check_check_export.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/check/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object tests/CMakeFiles/check_check_export.dir/check_check_log.c.o"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/check_check_export.dir/check_check_log.c.o -MF CMakeFiles/check_check_export.dir/check_check_log.c.o.d -o CMakeFiles/check_check_export.dir/check_check_log.c.o -c /home/qba/git/afb-libafb/check/tests/check_check_log.c

tests/CMakeFiles/check_check_export.dir/check_check_log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/check_check_export.dir/check_check_log.c.i"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/check/tests/check_check_log.c > CMakeFiles/check_check_export.dir/check_check_log.c.i

tests/CMakeFiles/check_check_export.dir/check_check_log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/check_check_export.dir/check_check_log.c.s"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/check/tests/check_check_log.c -o CMakeFiles/check_check_export.dir/check_check_log.c.s

tests/CMakeFiles/check_check_export.dir/check_check_fork.c.o: tests/CMakeFiles/check_check_export.dir/flags.make
tests/CMakeFiles/check_check_export.dir/check_check_fork.c.o: /home/qba/git/afb-libafb/check/tests/check_check_fork.c
tests/CMakeFiles/check_check_export.dir/check_check_fork.c.o: tests/CMakeFiles/check_check_export.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/check/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object tests/CMakeFiles/check_check_export.dir/check_check_fork.c.o"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/check_check_export.dir/check_check_fork.c.o -MF CMakeFiles/check_check_export.dir/check_check_fork.c.o.d -o CMakeFiles/check_check_export.dir/check_check_fork.c.o -c /home/qba/git/afb-libafb/check/tests/check_check_fork.c

tests/CMakeFiles/check_check_export.dir/check_check_fork.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/check_check_export.dir/check_check_fork.c.i"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/check/tests/check_check_fork.c > CMakeFiles/check_check_export.dir/check_check_fork.c.i

tests/CMakeFiles/check_check_export.dir/check_check_fork.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/check_check_export.dir/check_check_fork.c.s"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/check/tests/check_check_fork.c -o CMakeFiles/check_check_export.dir/check_check_fork.c.s

tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.o: tests/CMakeFiles/check_check_export.dir/flags.make
tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.o: /home/qba/git/afb-libafb/check/tests/check_check_export_main.c
tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.o: tests/CMakeFiles/check_check_export.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qba/git/afb-libafb/tis/build/check/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.o"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.o -MF CMakeFiles/check_check_export.dir/check_check_export_main.c.o.d -o CMakeFiles/check_check_export.dir/check_check_export_main.c.o -c /home/qba/git/afb-libafb/check/tests/check_check_export_main.c

tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/check_check_export.dir/check_check_export_main.c.i"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/qba/git/afb-libafb/check/tests/check_check_export_main.c > CMakeFiles/check_check_export.dir/check_check_export_main.c.i

tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/check_check_export.dir/check_check_export_main.c.s"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/qba/git/afb-libafb/check/tests/check_check_export_main.c -o CMakeFiles/check_check_export.dir/check_check_export_main.c.s

# Object files for target check_check_export
check_check_export_OBJECTS = \
"CMakeFiles/check_check_export.dir/check_check_sub.c.o" \
"CMakeFiles/check_check_export.dir/check_check_master.c.o" \
"CMakeFiles/check_check_export.dir/check_check_log.c.o" \
"CMakeFiles/check_check_export.dir/check_check_fork.c.o" \
"CMakeFiles/check_check_export.dir/check_check_export_main.c.o"

# External object files for target check_check_export
check_check_export_EXTERNAL_OBJECTS =

tests/check_check_export: tests/CMakeFiles/check_check_export.dir/check_check_sub.c.o
tests/check_check_export: tests/CMakeFiles/check_check_export.dir/check_check_master.c.o
tests/check_check_export: tests/CMakeFiles/check_check_export.dir/check_check_log.c.o
tests/check_check_export: tests/CMakeFiles/check_check_export.dir/check_check_fork.c.o
tests/check_check_export: tests/CMakeFiles/check_check_export.dir/check_check_export_main.c.o
tests/check_check_export: tests/CMakeFiles/check_check_export.dir/build.make
tests/check_check_export: src/libcheck.a
tests/check_check_export: tests/CMakeFiles/check_check_export.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qba/git/afb-libafb/tis/build/check/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable check_check_export"
	cd /home/qba/git/afb-libafb/tis/build/check/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/check_check_export.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/check_check_export.dir/build: tests/check_check_export
.PHONY : tests/CMakeFiles/check_check_export.dir/build

tests/CMakeFiles/check_check_export.dir/clean:
	cd /home/qba/git/afb-libafb/tis/build/check/tests && $(CMAKE_COMMAND) -P CMakeFiles/check_check_export.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/check_check_export.dir/clean

tests/CMakeFiles/check_check_export.dir/depend:
	cd /home/qba/git/afb-libafb/tis/build/check && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qba/git/afb-libafb/check /home/qba/git/afb-libafb/check/tests /home/qba/git/afb-libafb/tis/build/check /home/qba/git/afb-libafb/tis/build/check/tests /home/qba/git/afb-libafb/tis/build/check/tests/CMakeFiles/check_check_export.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/check_check_export.dir/depend

