#include "utils.hpp"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;
unsigned char to_byte(bool b[8])
{
    unsigned char c = 0;
    for (int i=0; i < 8; ++i)
        if (b[i])
            c |= 1 << i;
    return c;
}


void from_file(std::string fname, bool* A, int nrows, int ncols){
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


void initialize_file(std::string fname, int num_iterations, int nrows, int ncols){ 
    // TODO
}


void append_to_file(std::string fname, bool* A, int nrows, int ncols) {
    // TODO: change to append to file instead of creating new file
    bool* fromArray = new bool[8];
    int k = 0; 
    ofstream MyWriteFile(fname);
    for (int i = 0; i < nrows; i++) {
    	for (int j = 0; j < ncols; j++) {
    		if (k == 7){
    			k = 0;
    			//write to file
    			unsigned char c = to_byte(fromArray);
    			printf("writing to file %d\n", c);
    			MyWriteFile << c;    			
    			//reset boolFromArray maybe?	
    		}
    		else{
    			fromArray[k] = A[i*ncols + j];
    			k = k + 1;
    		}
    	}
    }
    MyWriteFile.close();
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
