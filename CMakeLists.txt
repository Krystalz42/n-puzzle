cmake_minimum_required(VERSION 3.12)
project(n_puzzle)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DN_PUZZLE_ROOT=\\\"${CMAKE_SOURCE_DIR}\\\" -DDN_PUZZLE_BUILD=\\\"${CMAKE_CURRENT_BINARY_DIR}\\\"")

set(BOOST_ROOT $ENV{HOME}/.brew/)
find_package(Boost QUIET COMPONENTS program_options thread filesystem)
if (Boost_FOUND)
    message("EXTERN : Boost finded")
elseif (NOT TARGET Boost::boost)
    message("EXTERN : Unable to find Boost, cloning...")
    if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init cmake/boost-cmake
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
    add_subdirectory(cmake/boost-cmake)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
set (SFML_ROOT "D:\\SFML-2.5.1")
find_package(SFML 2 REQUIRED network audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})

link_directories($ENV{HOME}/.brew/lib/)
include_directories($ENV{HOME}/.brew/include)

add_executable(n_puzzle
        srcs/cores/main.cpp
        srcs/resolver/KStar.hpp
        srcs/resolver/KStar.cpp
        srcs/parser/Parser.cpp
        srcs/parser/Parser.hpp
        srcs/visualizer/DisplaySfml.cpp
        srcs/visualizer/DisplaySfml.hpp
        srcs/visualizer/GridPuzzle.cpp
        srcs/visualizer/GridPuzzle.hpp
        srcs/visualizer/SfVectorInterpolate.cpp
        srcs/visualizer/SfVectorInterpolate.hpp
        incs/Grid.hpp
        incs/Position.hpp
        incs/Grid.cpp
        incs/PriorityQueue.hpp incs/IDisplay.hpp srcs/visualizer/GridSpriteManager.cpp srcs/visualizer/GridSpriteManager.hpp srcs/visualizer/Visualizer.cpp srcs/visualizer/Visualizer.hpp srcs/visualizer/Text.cpp srcs/visualizer/Text.hpp srcs/visualizer/TimeLogic.cpp srcs/visualizer/TimeLogic.hpp)

include_directories(incs)
include_directories(srcs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3")

target_link_libraries(n_puzzle PRIVATE Boost::filesystem)
target_link_libraries(n_puzzle PRIVATE Boost::thread)
target_link_libraries(n_puzzle PRIVATE Boost::program_options)
target_link_libraries(n_puzzle PRIVATE ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})