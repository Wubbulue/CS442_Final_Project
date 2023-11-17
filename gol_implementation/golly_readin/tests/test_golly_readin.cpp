#include <cstdio>

#include <readin.hpp>
#include <utils.hpp>

int main(int argc, char** argv) {
    printf("TESTING READIN \n\n");
    int nrows = 50;
    int ncols = 50;
    
    const char *filename = GOLLY_TEST_FILE_PATH;
    
    bool * A = loadPattern(nrows, ncols, filename);
    
    print_matrix(A, nrows, ncols);
}

