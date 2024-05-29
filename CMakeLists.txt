cmake_minimum_required(VERSION 3.10)

# We define our project 
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory thiks is in our main.cpp file 
include_directories(include)

# Here we are creating an executable named MyProject which will run main.cpp 
add_executable(MyProject src/main.cpp)

# Searches for the libcurl library.
find_package(CURL REQUIRED)

#  Links the libcurl library to the executable. 
target_link_libraries(MyProject CURL::libcurl)
