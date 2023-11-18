#include "gol_serial.hpp"
#include <utils.hpp>
#include <iostream>
#include <random>
#include <functional>
#include <chrono>


int main(int argc, char** argv) {

    //Cpp chrono garbage
    using Clock = std::chrono::steady_clock;
    using std::chrono::time_point;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;


    int nrows = 1024;
    int ncols = 1024;
    bool* A = new bool[nrows*ncols];
    auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
    for (int i=0; i < nrows; i++) {
        for (int j=0; j < ncols; j++) {
            A[i*ncols + j] = gen();
        }
    }

    const bool writeToFile = true;

    int n_iterations = 100;

    printf("Running a GOL Simulation with %d rows, %d cols, and %d interations\n",256,256,n_iterations);

    if (writeToFile) {
        printf("Warning, file writing is enabled, which could impact performance\n");
    }

    
    std::string out_filename = "out_test.mat";
    if (writeToFile) {
		initialize_file(out_filename, n_iterations,nrows,ncols);
    }

    time_point<Clock> start = Clock::now();
    run(A, n_iterations, nrows, ncols, writeToFile, false,out_filename);
    time_point<Clock> end = Clock::now();
    milliseconds diff = duration_cast<milliseconds>(end - start);
    std::cout << "Simulation took: " << diff.count() << " miliseconds" << std::endl;

 
    delete[] A;
    return 0;
}
