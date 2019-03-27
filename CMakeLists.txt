cmake_minimum_required(VERSION 3.13)
project(n_puzzle)

set(CMAKE_CXX_STANDARD 17)

add_executable(n_puzzle
        srcs/cores/main.cpp
        srcs/resolver/KStar.hpp
        srcs/resolver/KStar.cpp
        srcs/parser/Parser.cpp
        srcs/parser/Parser.hpp
        incs/Grid.hpp
        incs/Position.hpp
        incs/PriorityQueue.hpp incs/Grid.cpp)

include_directories(incs)
include_directories(srcs)
find_package(Boost QUIET COMPONENTS program_options thread filesystem)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3")
target_link_libraries(n_puzzle PRIVATE Boost::filesystem)
target_link_libraries(n_puzzle PRIVATE Boost::thread)
target_link_libraries(n_puzzle PRIVATE Boost::program_options)