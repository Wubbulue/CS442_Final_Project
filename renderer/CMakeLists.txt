cmake_minimum_required(VERSION 3.17)
project(Renderer)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


# Link
add_executable(Renderer renderer.cpp)
add_executable(Make_Example make_example_mat.cpp)
target_link_libraries(Renderer ${OpenCV_LIBS})
target_link_libraries(Make_Example ${OpenCV_LIBS})