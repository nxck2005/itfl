cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(itfl VERSION 0.1.2)

add_executable(itfl src/itfl.cpp)

target_include_directories(itfl PUBLIC
    ${CMAKE_SOURCE_DIR}/lib
)
