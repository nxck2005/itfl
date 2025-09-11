cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(itfl VERSION 0.2.0)

add_executable(itfl src/itfl.cpp lib/sha256.cpp)

target_include_directories(itfl PUBLIC
    ${CMAKE_SOURCE_DIR}/lib
)

target_compile_definitions(itfl PRIVATE NDEBUG)

target_compile_options(itfl PRIVATE -O3)

# link time optimization
set_property(TARGET itfl PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
