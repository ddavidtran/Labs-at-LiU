cmake_minimum_required(VERSION 3.10)
project(Lab4_particlesim)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab4_particlesim
        coordinate.h
        definitions.h
        main_sequential.cpp
        physics.cpp
        physics.h)
