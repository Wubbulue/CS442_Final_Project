#include "utils.hpp"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

#include "readin.hpp"

using namespace std;
unsigned char to_byte(bool b[8])
{
    unsigned char c = 0;
    for (int i=0; i < 8; ++i)
        if (b[i])
            c |= 1 << i;
    return c;
}

void initialize_board_randomly(bool* A, int nrows, int ncols, int seed) {
    std::mt19937 engine;
    engine.seed(seed);
    auto gen = std::bind(std::uniform_int_distribution<>(0, 1), engine);
    for (int i=0; i < nrows; i++) {
        for (int j=0; j < ncols; j++) {
            A[i*ncols + j] = gen();
        }
    }
}


void initilize_board_from_file(std::string fname, bool* A, int nrows, int ncols){
    A = loadPattern(nrows, ncols, fname.c_str()); 
}

void from_file(std::string fname, bool* A, int nrows, int ncols){
	printf("reading from file \n");
    std::ifstream in{fname, std::ios::in | std::ios::binary };
    std::vector<char> contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    //now turn contents into initial board A
    printf("byte is %d\n " , contents[0]);
    printf("size vector is %ld\n", contents.size());
    int k = 0;
    
    for(int i = 0; i < contents.size(); i++){
      unsigned char currentByte = contents[i];
      ///convert byte to bool A components
      for(int j = 0; j < 8; j ++){
      	A[k] = currentByte & 1;
      	currentByte = currentByte >> 1;
      	k = k + 1;   	
      }
  
    }

    print_matrix(A,nrows,ncols);
    
}


void initialize_file(std::string fname, uint32_t num_iterations, uint32_t nrows, uint32_t ncols){ 
    ofstream outFile(fname,std::ios::binary);

	outFile.write(reinterpret_cast<const char*>(&num_iterations), sizeof(uint32_t));
    outFile.write(reinterpret_cast<const char*>(&nrows), sizeof(uint32_t));
    outFile.write(reinterpret_cast<const char*>(&ncols), sizeof(uint32_t));

}


void append_to_file(std::string fname, bool* A, int nrows, int ncols) {
    bool* fromArray = new bool[8];
    int k = 0; 
    
    std::ofstream outFile;
    outFile.open(fname,std::ios_base::app|std::ios::binary);
    
    
    for (int i = 0; i < nrows; i++) {
    	for (int byteCol = 0; byteCol < ncols/8; byteCol++) {
            for (int bit = 0; bit < 8; bit++) {
                fromArray[bit] = A[i * ncols + byteCol * 8 + bit];
            }
			uint8_t c = to_byte(fromArray);
            outFile.write(reinterpret_cast<const char*>(&c), 1);
    	}
    }

    delete[] fromArray;
    outFile.close();
}



void print_matrix(bool* A, int nrows, int ncols) {
    for (int i=0; i < nrows; i++) {
        for (int j=0; j < ncols; j++) {
            if (A[i*ncols + j]) {
                printf("1 ");
            }
            else {
                printf("0 ");
            }
        }
        printf("\n");
    }    
    printf("\n");
}

void parse_args(int argc, char *argv[], int& n_iter, int& n_dims, std::string& file_path,
                bool& file_io_flag, std::string& save_file, int& seed) {
    n_iter = -1; // required arg
    n_dims = -1; 
    file_path = ""; // optional (default)
    file_io_flag = false; 
    save_file = "";
    seed = 42;

    // parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            std::string option(argv[i]);
            if (option == "-n_iter" && i + 1 < argc) {
                n_iter = std::atoi(argv[++i]);
            } else if (option == "-n_dims" && i + 1 < argc) {
                n_dims = std::atoi(argv[++i]);
            } else if (option == "-file_path" && i + 1 < argc) {
                file_path = argv[++i];
            } else if (option == "-file_io_flag") {
                file_io_flag = true;
            } else if (option == "-save_file" && i + 1 < argc) {
                save_file = argv[++i];
            } else if (option == "-seed" && i + 1 < argc) {
                seed = std::atoi(argv[++i]);
            } else {
                std::cerr << "Unknown option or missing argument: " << option << std::endl;
                std::exit(1);
            }
        }
    }

    // check if required arguments are provided
    if (n_iter == -1 || n_dims == -1) {
        std::cerr << "Required arguments n_iter and n_dims are missing." << std::endl;
        std::exit(1);
    }
}
