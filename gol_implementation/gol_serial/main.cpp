#include "gol_serial.hpp"
#include "readin.hpp"

#include <utils.hpp>
#include <iostream>
#include <random>
#include <functional>
#include <chrono>


int main(int argc, char** argv) {
    // parsing command-line args
    int n_iter, n_dims, seed;
    std::string file_path, save_file;
    bool file_io_flag;
    parse_args(argc, argv, n_iter, n_dims, file_path, file_io_flag, save_file, seed);
    // print out to check
    printf("n_iter: %d | n_dims: %d | seed: %d\n", n_iter, n_dims, seed);
    std::cout << "file_path: " << file_path << std::endl;
    std::cout << "file_io_flag: " << file_io_flag << std::endl;
    std::cout << "save_file: " << save_file << std::endl;


    //Cpp chrono garbage
    using Clock = std::chrono::steady_clock;
    using std::chrono::time_point;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;


    int nrows = n_dims;
    int ncols = n_dims;
    bool* A = new bool[nrows*ncols];
    if (file_path == "") {
        // no file given, random initialize
        initialize_board_randomly(A, nrows, ncols, seed);
    }
    else {
        initilize_board_from_file(file_path, A, nrows, ncols);
    }

    const bool writeToFile = file_io_flag;

    int n_iterations = n_iter;

    printf("Running a GOL Simulation with %d rows, %d cols, and %d interations\n",nrows,ncols,n_iterations);

    if (writeToFile) {
        printf("Warning, file writing is enabled, which could impact performance\n");
    }

    
    if (writeToFile) {
		initialize_file(save_file, n_iterations,nrows,ncols);
    }


    time_point<Clock> start = Clock::now();
    run(A, n_iterations, nrows, ncols, writeToFile, false, save_file);
    time_point<Clock> end = Clock::now();
    milliseconds diff = duration_cast<milliseconds>(end - start);
    std::cout << "Simulation took: " << diff.count() << " miliseconds" << std::endl;

 
    delete[] A;
    return 0;
}
