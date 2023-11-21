add_definitions(-DGOLLY_TEST_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../patterns/test.pat")

add_executable(Gol_Serial main.cpp gol_serial.cpp gol_serial.hpp)
target_link_libraries(Gol_Serial PUBLIC utils Golly_Readin)
