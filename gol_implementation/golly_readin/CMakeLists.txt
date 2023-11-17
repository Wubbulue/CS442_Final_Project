add_library(Golly_Readin readin.cpp readin.hpp)
target_include_directories(Golly_Readin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(tests)
