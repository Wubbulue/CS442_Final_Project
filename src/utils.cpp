#include <stdio.h>

#include "utils.hpp"


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


void to_file(std::string fname, bool* A, int nrows, int ncols) {
    // TODO
}
