cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project (circuit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(EIGEN_ROOT_DIR "/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3/")

include_directories(
    ${EIGEN_ROOT_DIR}
)

add_executable (circuit circuit_main.cpp)