cmake_minimum_required(VERSION 3.10)

# We define our project 
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# Here we are creating an executable named MyProject from the source file src/main.cpp.
add_executable(MyProject src/main.cpp)

# Searches for the libcurl library.
find_package(CURL REQUIRED)

#  Links the libcurl library to the executable. 
target_link_libraries(MyProject CURL::libcurl)
