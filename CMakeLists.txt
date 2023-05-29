cmake_minimum_required(VERSION 3.17)
project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_COLOR_MAKEFILE ON)

add_executable(${PROJECT_NAME} main.cpp
    # source/CollatzSolution.h
    # source/CollatzSolver.h
    # source/CollatzSolver.cpp)
    source/Database.cpp)
    # source/ElementsHolder.h 
    # source/ThreadPool.h 
    # source/ThreadPool.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE gmp gmpxx cassandra)