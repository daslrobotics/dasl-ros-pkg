# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dasl/rosbuild_ws/roscopter/mavlink

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dasl/rosbuild_ws/roscopter/mavlink/build

# Utility rule file for minimal.xml-v1.0.

# Include the progress variables for this target.
include CMakeFiles/minimal.xml-v1.0.dir/progress.make

CMakeFiles/minimal.xml-v1.0: minimal.xml-v1.0-stamp

minimal.xml-v1.0-stamp: ../message_definitions/v1.0/minimal.xml
minimal.xml-v1.0-stamp: ../pymavlink/generator/mavgen.py
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dasl/rosbuild_ws/roscopter/mavlink/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating minimal.xml-v1.0-stamp"
	/usr/bin/python /home/dasl/rosbuild_ws/roscopter/mavlink/pymavlink/generator/mavgen.py --lang=C --wire-protocol=1.0 --output=include/v1.0 /home/dasl/rosbuild_ws/roscopter/mavlink/message_definitions/v1.0/minimal.xml
	touch minimal.xml-v1.0-stamp

minimal.xml-v1.0: CMakeFiles/minimal.xml-v1.0
minimal.xml-v1.0: minimal.xml-v1.0-stamp
minimal.xml-v1.0: CMakeFiles/minimal.xml-v1.0.dir/build.make
.PHONY : minimal.xml-v1.0

# Rule to build all files generated by this target.
CMakeFiles/minimal.xml-v1.0.dir/build: minimal.xml-v1.0
.PHONY : CMakeFiles/minimal.xml-v1.0.dir/build

CMakeFiles/minimal.xml-v1.0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/minimal.xml-v1.0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/minimal.xml-v1.0.dir/clean

CMakeFiles/minimal.xml-v1.0.dir/depend:
	cd /home/dasl/rosbuild_ws/roscopter/mavlink/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dasl/rosbuild_ws/roscopter/mavlink /home/dasl/rosbuild_ws/roscopter/mavlink /home/dasl/rosbuild_ws/roscopter/mavlink/build /home/dasl/rosbuild_ws/roscopter/mavlink/build /home/dasl/rosbuild_ws/roscopter/mavlink/build/CMakeFiles/minimal.xml-v1.0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/minimal.xml-v1.0.dir/depend

