cmake_minimum_required(VERSION 3.4...3.18)
project(cmake_example)

add_subdirectory(pybind11)
file(GLOB SOURCES "src/*.cpp")
# pybind11_add_module(cmake_example src/main.cpp src/Class.cpp src/SmartPtr.cpp src/SmartPtr2.cpp src/Win.cpp)
pybind11_add_module(cmake_example ${SOURCES})

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(cmake_example
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

# enable asan!!
target_compile_options(cmake_example PRIVATE /fsanitize=address /Zi)