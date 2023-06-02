cmake_minimum_required(VERSION 3.17)
project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_COLOR_MAKEFILE ON)

add_executable(${PROJECT_NAME} main.cpp
    source/ThreadPool.cpp
    source/ElementsHolder.cpp
    source/CollatzSolution.cpp
    source/CollatzSolver.cpp
    source/Database.cpp
    source/DatabaseInstance.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE gmp gmpxx cassandra)