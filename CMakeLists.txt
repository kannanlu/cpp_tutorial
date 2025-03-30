cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project (circuit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EIGEN_ROOT_DIR "/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3/")

# Include Eigen3 headers
include_directories(
    ${EIGEN_ROOT_DIR}
)

# Add executable
add_executable (circuit circuit_main.cpp)

# Enable better warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(circuit PRIVATE -Wall -Wextra -pedantic)
endif()