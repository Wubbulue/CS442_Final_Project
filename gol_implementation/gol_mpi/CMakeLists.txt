find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(CMAKE_BUILD_TYPE Release)

add_executable(gol_mpi gol_mpi.cpp)
target_link_libraries(gol_mpi ${MPI_LIBRARIES} utils)

add_executable(gol_shared gol_shared.cpp)
target_link_libraries(gol_shared ${MPI_LIBRARIES} utils)

