cmake_minimum_required(VERSION 3.5)

project(Interpreter)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")

set(source_dir "${PROJECT_SOURCE_DIR}/src")
set(include_dir "${PROJECT_SOURCE_DIR}/include")

file(GLOB source_files "${source_dir}/*.cpp")
file(GLOB include_files "${include_dir}/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin.bin")

add_executable(Interpreter ${source_files} ${include_files})


