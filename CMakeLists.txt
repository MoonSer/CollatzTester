cmake_minimum_required(VERSION 3.17)
project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_COLOR_MAKEFILE ON)

find_package(Boost 1.56.0 REQUIRED) 
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
    source/Database.h 
    source/Database.cpp 
    source/ElementsHolder.h 
    source/ThreadPool.h 
    source/ThreadPool.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE gmp gmpxx mongo::mongocxx_shared mongo::bsoncxx_shared)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBMONGOCXX_INCLUDE_DIR} ${LIBBSONCXX_INCLUDE_DIR})
