cmake_minimum_required(VERSION 3.15)
project(Gol_Implementation)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(utils)
add_subdirectory(golly_readin)
add_subdirectory(gol_serial)
add_subdirectory(gol_mpi)

