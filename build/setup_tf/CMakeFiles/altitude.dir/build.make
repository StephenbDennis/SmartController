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
CMAKE_SOURCE_DIR = /home/stephen/catkin_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stephen/catkin_ws/build

# Include any dependencies generated for this target.
include setup_tf/CMakeFiles/altitude.dir/depend.make

# Include the progress variables for this target.
include setup_tf/CMakeFiles/altitude.dir/progress.make

# Include the compile flags for this target's objects.
include setup_tf/CMakeFiles/altitude.dir/flags.make

setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o: setup_tf/CMakeFiles/altitude.dir/flags.make
setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o: /home/stephen/catkin_ws/src/setup_tf/src/altitude.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stephen/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o"
	cd /home/stephen/catkin_ws/build/setup_tf && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/altitude.dir/src/altitude.cpp.o -c /home/stephen/catkin_ws/src/setup_tf/src/altitude.cpp

setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/altitude.dir/src/altitude.cpp.i"
	cd /home/stephen/catkin_ws/build/setup_tf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stephen/catkin_ws/src/setup_tf/src/altitude.cpp > CMakeFiles/altitude.dir/src/altitude.cpp.i

setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/altitude.dir/src/altitude.cpp.s"
	cd /home/stephen/catkin_ws/build/setup_tf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stephen/catkin_ws/src/setup_tf/src/altitude.cpp -o CMakeFiles/altitude.dir/src/altitude.cpp.s

setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.requires:

.PHONY : setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.requires

setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.provides: setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.requires
	$(MAKE) -f setup_tf/CMakeFiles/altitude.dir/build.make setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.provides.build
.PHONY : setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.provides

setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.provides.build: setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o


# Object files for target altitude
altitude_OBJECTS = \
"CMakeFiles/altitude.dir/src/altitude.cpp.o"

# External object files for target altitude
altitude_EXTERNAL_OBJECTS =

/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: setup_tf/CMakeFiles/altitude.dir/build.make
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libtf.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libtf2_ros.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libactionlib.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libmessage_filters.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libroscpp.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_filesystem.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_signals.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libxmlrpcpp.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libtf2.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libroscpp_serialization.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/librosconsole.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/liblog4cxx.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_regex.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/librostime.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /opt/ros/kinetic/lib/libcpp_common.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_system.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_thread.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_chrono.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_date_time.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libboost_atomic.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libpthread.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: /usr/lib/i386-linux-gnu/libconsole_bridge.so
/home/stephen/catkin_ws/devel/lib/setup_tf/altitude: setup_tf/CMakeFiles/altitude.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stephen/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/stephen/catkin_ws/devel/lib/setup_tf/altitude"
	cd /home/stephen/catkin_ws/build/setup_tf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/altitude.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
setup_tf/CMakeFiles/altitude.dir/build: /home/stephen/catkin_ws/devel/lib/setup_tf/altitude

.PHONY : setup_tf/CMakeFiles/altitude.dir/build

setup_tf/CMakeFiles/altitude.dir/requires: setup_tf/CMakeFiles/altitude.dir/src/altitude.cpp.o.requires

.PHONY : setup_tf/CMakeFiles/altitude.dir/requires

setup_tf/CMakeFiles/altitude.dir/clean:
	cd /home/stephen/catkin_ws/build/setup_tf && $(CMAKE_COMMAND) -P CMakeFiles/altitude.dir/cmake_clean.cmake
.PHONY : setup_tf/CMakeFiles/altitude.dir/clean

setup_tf/CMakeFiles/altitude.dir/depend:
	cd /home/stephen/catkin_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stephen/catkin_ws/src /home/stephen/catkin_ws/src/setup_tf /home/stephen/catkin_ws/build /home/stephen/catkin_ws/build/setup_tf /home/stephen/catkin_ws/build/setup_tf/CMakeFiles/altitude.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : setup_tf/CMakeFiles/altitude.dir/depend

