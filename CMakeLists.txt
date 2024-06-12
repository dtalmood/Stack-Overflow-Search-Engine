cmake_minimum_required(VERSION 3.10)

# Define the project
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include) 
include_directories("${CMAKE_SOURCE_DIR}/Libraries/mongo-cxx-driver/3.10.1/include/mongocxx/v_noabi/")
include_directories("${CMAKE_SOURCE_DIR}/Libraries/mongo-cxx-driver/3.10.1/include/bsoncxx/v_noabi/")
include_directories("${CMAKE_SOURCE_DIR}/Libraries/mongo-cxx-driver/3.10.1/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc")
include_directories("${CMAKE_SOURCE_DIR}/Libraries/bcrypt/1.1/include")


# Find and link MongoDB C++ driver libraries
find_library(MONGOCXX_LIB mongocxx HINTS "${CMAKE_SOURCE_DIR}/library/lib")
find_library(BSONCXX_LIB bsoncxx HINTS "${CMAKE_SOURCE_DIR}/library/lib")
find_library(LIBBCRYPT_LIB bcrypt HINTS "${CMAKE_SOURCE_DIR}/library/lib")

# Create an executable named MyProject which will run main.cpp
add_executable(MyProject src/main.cpp src/authentication.cpp)

# Link MongoDB libraries
target_link_libraries(MyProject ${MONGOCXX_LIB} ${BSONCXX_LIB} ${LIBBCRYPT_LIB})

# Searches for the libcurl library
find_package(CURL REQUIRED)

# Links the libcurl library to the executable
target_link_libraries(MyProject CURL::libcurl)
