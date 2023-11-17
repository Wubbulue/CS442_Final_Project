add_definitions(-DGOLLY_TEST_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test.pat")
add_executable(test_golly_readin test_golly_readin.cpp)
target_link_libraries(test_golly_readin PUBLIC Golly_Readin utils)
