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
CMAKE_SOURCE_DIR = /home/mboujemaoui/Developer/abseil-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mboujemaoui/Developer/abseil-cpp

# Include any dependencies generated for this target.
include absl/types/CMakeFiles/absl_span.dir/depend.make

# Include the progress variables for this target.
include absl/types/CMakeFiles/absl_span.dir/progress.make

# Include the compile flags for this target's objects.
include absl/types/CMakeFiles/absl_span.dir/flags.make

absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o: absl/types/CMakeFiles/absl_span.dir/flags.make
absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o: absl/types/absl_span_header_only_dummy.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mboujemaoui/Developer/abseil-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o"
	cd /home/mboujemaoui/Developer/abseil-cpp/absl/types && /usr/lib/ccache/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o -c /home/mboujemaoui/Developer/abseil-cpp/absl/types/absl_span_header_only_dummy.cc

absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.i"
	cd /home/mboujemaoui/Developer/abseil-cpp/absl/types && /usr/lib/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mboujemaoui/Developer/abseil-cpp/absl/types/absl_span_header_only_dummy.cc > CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.i

absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.s"
	cd /home/mboujemaoui/Developer/abseil-cpp/absl/types && /usr/lib/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mboujemaoui/Developer/abseil-cpp/absl/types/absl_span_header_only_dummy.cc -o CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.s

absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.requires:

.PHONY : absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.requires

absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.provides: absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.requires
	$(MAKE) -f absl/types/CMakeFiles/absl_span.dir/build.make absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.provides.build
.PHONY : absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.provides

absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.provides.build: absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o


# Object files for target absl_span
absl_span_OBJECTS = \
"CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o"

# External object files for target absl_span
absl_span_EXTERNAL_OBJECTS =

absl/types/libabsl_span.a: absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o
absl/types/libabsl_span.a: absl/types/CMakeFiles/absl_span.dir/build.make
absl/types/libabsl_span.a: absl/types/CMakeFiles/absl_span.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mboujemaoui/Developer/abseil-cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_span.a"
	cd /home/mboujemaoui/Developer/abseil-cpp/absl/types && $(CMAKE_COMMAND) -P CMakeFiles/absl_span.dir/cmake_clean_target.cmake
	cd /home/mboujemaoui/Developer/abseil-cpp/absl/types && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_span.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
absl/types/CMakeFiles/absl_span.dir/build: absl/types/libabsl_span.a

.PHONY : absl/types/CMakeFiles/absl_span.dir/build

absl/types/CMakeFiles/absl_span.dir/requires: absl/types/CMakeFiles/absl_span.dir/absl_span_header_only_dummy.cc.o.requires

.PHONY : absl/types/CMakeFiles/absl_span.dir/requires

absl/types/CMakeFiles/absl_span.dir/clean:
	cd /home/mboujemaoui/Developer/abseil-cpp/absl/types && $(CMAKE_COMMAND) -P CMakeFiles/absl_span.dir/cmake_clean.cmake
.PHONY : absl/types/CMakeFiles/absl_span.dir/clean

absl/types/CMakeFiles/absl_span.dir/depend:
	cd /home/mboujemaoui/Developer/abseil-cpp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mboujemaoui/Developer/abseil-cpp /home/mboujemaoui/Developer/abseil-cpp/absl/types /home/mboujemaoui/Developer/abseil-cpp /home/mboujemaoui/Developer/abseil-cpp/absl/types /home/mboujemaoui/Developer/abseil-cpp/absl/types/CMakeFiles/absl_span.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : absl/types/CMakeFiles/absl_span.dir/depend

