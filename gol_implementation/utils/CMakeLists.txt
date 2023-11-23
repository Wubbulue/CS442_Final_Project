find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(CMAKE_BUILD_TYPE Release)

add_library(utils utils.cpp utils.hpp)

target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(utils PUBLIC MPI::MPI_CXX Golly_Readin)
