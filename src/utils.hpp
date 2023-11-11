#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <fstream>
#include <string>
// load contents from fname into A
void from_file(std::string fname, bool* A, int nrows, int ncols);
// store contents of A into the provided file
void to_file(std::string fname, bool* A, int nrows, int ncols);
// convert boolean arr to byte
unsigned char to_byte(bool b[8]);
// for debugging purposes
void print_matrix(bool* A, int nrows, int ncols);
#endif
