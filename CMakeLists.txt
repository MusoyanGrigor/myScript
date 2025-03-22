cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(myScript)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Add the source files
add_executable(myScript
        src/main.cpp
        src/interpreter.cpp
)

# Include directories
target_include_directories(myScript PUBLIC ${CMAKE_SOURCE_DIR}/src)