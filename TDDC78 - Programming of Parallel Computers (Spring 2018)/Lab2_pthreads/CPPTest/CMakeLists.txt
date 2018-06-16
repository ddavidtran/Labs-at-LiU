cmake_minimum_required(VERSION 3.9)
project(Lab2)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_CXX_STANDARD 17)

add_executable(blur
        blurmain.cpp
        blurfilter.cpp
        blurfilter.h
        )


add_executable(thresh
        thresmain.cpp
        thresfilter.cpp
        thresfilter.h
        )