# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alexandreroulin/n-puzzle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alexandreroulin/n-puzzle/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/n_puzzle.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/n_puzzle.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/n_puzzle.dir/flags.make

CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.o: CMakeFiles/n_puzzle.dir/flags.make
CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.o: ../srcs/cores/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandreroulin/n-puzzle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.o -c /Users/alexandreroulin/n-puzzle/srcs/cores/main.cpp

CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandreroulin/n-puzzle/srcs/cores/main.cpp > CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.i

CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandreroulin/n-puzzle/srcs/cores/main.cpp -o CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.s

CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.o: CMakeFiles/n_puzzle.dir/flags.make
CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.o: ../srcs/resolver/KStar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandreroulin/n-puzzle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.o -c /Users/alexandreroulin/n-puzzle/srcs/resolver/KStar.cpp

CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandreroulin/n-puzzle/srcs/resolver/KStar.cpp > CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.i

CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandreroulin/n-puzzle/srcs/resolver/KStar.cpp -o CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.s

CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.o: CMakeFiles/n_puzzle.dir/flags.make
CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.o: ../srcs/parser/Parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandreroulin/n-puzzle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.o -c /Users/alexandreroulin/n-puzzle/srcs/parser/Parser.cpp

CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandreroulin/n-puzzle/srcs/parser/Parser.cpp > CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.i

CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandreroulin/n-puzzle/srcs/parser/Parser.cpp -o CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.s

CMakeFiles/n_puzzle.dir/incs/Grid.cpp.o: CMakeFiles/n_puzzle.dir/flags.make
CMakeFiles/n_puzzle.dir/incs/Grid.cpp.o: ../incs/Grid.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandreroulin/n-puzzle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/n_puzzle.dir/incs/Grid.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/n_puzzle.dir/incs/Grid.cpp.o -c /Users/alexandreroulin/n-puzzle/incs/Grid.cpp

CMakeFiles/n_puzzle.dir/incs/Grid.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/n_puzzle.dir/incs/Grid.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandreroulin/n-puzzle/incs/Grid.cpp > CMakeFiles/n_puzzle.dir/incs/Grid.cpp.i

CMakeFiles/n_puzzle.dir/incs/Grid.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/n_puzzle.dir/incs/Grid.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandreroulin/n-puzzle/incs/Grid.cpp -o CMakeFiles/n_puzzle.dir/incs/Grid.cpp.s

# Object files for target n_puzzle
n_puzzle_OBJECTS = \
"CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.o" \
"CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.o" \
"CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.o" \
"CMakeFiles/n_puzzle.dir/incs/Grid.cpp.o"

# External object files for target n_puzzle
n_puzzle_EXTERNAL_OBJECTS =

n_puzzle: CMakeFiles/n_puzzle.dir/srcs/cores/main.cpp.o
n_puzzle: CMakeFiles/n_puzzle.dir/srcs/resolver/KStar.cpp.o
n_puzzle: CMakeFiles/n_puzzle.dir/srcs/parser/Parser.cpp.o
n_puzzle: CMakeFiles/n_puzzle.dir/incs/Grid.cpp.o
n_puzzle: CMakeFiles/n_puzzle.dir/build.make
n_puzzle: /usr/local/lib/libboost_filesystem-mt.dylib
n_puzzle: /usr/local/lib/libboost_thread-mt.dylib
n_puzzle: /usr/local/lib/libboost_program_options-mt.dylib
n_puzzle: /usr/local/lib/libboost_chrono-mt.dylib
n_puzzle: /usr/local/lib/libboost_system-mt.dylib
n_puzzle: /usr/local/lib/libboost_date_time-mt.dylib
n_puzzle: /usr/local/lib/libboost_atomic-mt.dylib
n_puzzle: CMakeFiles/n_puzzle.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/alexandreroulin/n-puzzle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable n_puzzle"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/n_puzzle.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/n_puzzle.dir/build: n_puzzle

.PHONY : CMakeFiles/n_puzzle.dir/build

CMakeFiles/n_puzzle.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/n_puzzle.dir/cmake_clean.cmake
.PHONY : CMakeFiles/n_puzzle.dir/clean

CMakeFiles/n_puzzle.dir/depend:
	cd /Users/alexandreroulin/n-puzzle/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alexandreroulin/n-puzzle /Users/alexandreroulin/n-puzzle /Users/alexandreroulin/n-puzzle/cmake-build-debug /Users/alexandreroulin/n-puzzle/cmake-build-debug /Users/alexandreroulin/n-puzzle/cmake-build-debug/CMakeFiles/n_puzzle.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/n_puzzle.dir/depend

