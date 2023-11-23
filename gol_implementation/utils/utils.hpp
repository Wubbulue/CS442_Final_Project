#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <functional>
#include <mpi.h>

// load contents from fname into A
void from_file(std::string fname, bool* A, int nrows, int ncols);
// initialize file with header, num_frames==num_iterations of GOL
void initialize_file(std::string fname, uint32_t num_iterations, uint32_t nrows, uint32_t ncols);
// store contents of A into the provided file
void append_to_file(std::string fname, bool* A, int nrows, int ncols);
// convert boolean arr to byte
unsigned char to_byte(bool b[8]);
// for debugging purposes
void print_matrix(bool* A, int nrows, int ncols);
// for parsing command-line args
void parse_args(int argc, char *argv[], int& n_iter, int& n_dims, std::string& file_path,
                bool& file_io_flag, std::string& save_file, int& seed);
void initilize_board_from_file(std::string fname, bool* A, int nrows, int ncols);
void initialize_board_randomly(bool* A, int nrows, int ncols, int seed);
void scatter(int num_procs, int id, bool* A, bool* B, int root, int global_n, int dim, int n, MPI_Datatype row_type);
void gather(int num_procs, int id, bool* A, bool* B, int root, int global_n, int dim, int n, MPI_Datatype row_type);
#endif
