#include "gol.hpp"
#include "utils.hpp"


int main(int argc, char** argv) {
    printf("TESTING GOL LOGIC\n\n");
    int nrows = 4;
    int ncols = 4;
    bool* A = new bool[nrows*ncols];
    for (int i=0; i < nrows; i++) {
        for (int j=0; j < ncols; j++) {
            A[i*ncols + j] = false;
        }
    }
    A[1*ncols + 1] = true;
    A[1*ncols + 2] = true;
    A[2*ncols + 1] = true;
    A[2*ncols + 2] = true;
    int n_iterations = 10;
    run(A, n_iterations, nrows, ncols, false, true);
 
    printf("\nTESTING UTILS (FILE SAVE/LOAD)\n\n");
    bool* testBytes = new bool[8];
    append_to_file("filename.txt",A,nrows,ncols);
    from_file("filename.txt",A,nrows,ncols);
    print_matrix(A,nrows,ncols);
    delete[] A;
    return 0;
}
