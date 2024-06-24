cmake_minimum_required(VERSION 3.10)

# Define the project
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include) 
include_directories("/opt/homebrew/include/mongocxx/v_noabi/")
include_directories("/opt/homebrew/include/bsoncxx/v_noabi/")
include_directories("/opt/homebrew/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc")
include_directories("/usr/local/include/libbcrypt/include/bcrypt") # Updated path for libbcrypt

# Ensure policies are set correctly
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif()

# Find and link MongoDB C++ driver libraries
find_library(MONGOCXX_LIB mongocxx HINTS "/opt/homebrew/lib" "/opt/homebrew/lib/mongocxx-3.10.1" "/opt/homebrew/lib/mongocxx-3.10.1/lib")
find_library(BSONCXX_LIB bsoncxx HINTS "/opt/homebrew/lib" "/opt/homebrew/lib/bsoncxx-3.10.1" "/opt/homebrew/lib/bsoncxx-3.10.1/lib")
find_library(LIBBCRYPT_LIB bcrypt HINTS "/usr/local/lib" "/usr/local/libbcrypt/lib") # Updated path for libbcrypt

# Check if libraries were found
if(NOT MONGOCXX_LIB)
  message(FATAL_ERROR "Mongocxx library not found")
endif()
if(NOT BSONCXX_LIB)
  message(FATAL_ERROR "Bsoncxx library not found")
endif()
if(NOT LIBBCRYPT_LIB)
  message(FATAL_ERROR "Bcrypt library not found")
endif()

# Create an executable named MyProject which will run main.cpp
add_executable(MyProject src/main.cpp src/authentication.cpp)

# Link MongoDB libraries
target_link_libraries(MyProject ${MONGOCXX_LIB} ${BSONCXX_LIB} ${LIBBCRYPT_LIB})

# Searches for the libcurl library
find_package(CURL REQUIRED)

# Links the libcurl library to the executable
target_link_libraries(MyProject CURL::libcurl)
