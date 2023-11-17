#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "gol_serial.hpp"
#include <utils.hpp>


// run will evolve, copy data to A_old, and save matrix (if SAVE_TO_FILE flag), then repeat.
// if DEBUG then print matrix every iteration
void run(bool* A, int num_iterations, int nrows, int ncols, bool SAVE_TO_FILE, bool DEBUG, std::string save_file) {
    unsigned int size_A = nrows*ncols;
    bool* A_old = new bool[size_A];
    memcpy(A_old, A, size_A*sizeof(bool));
    if (SAVE_TO_FILE) {
        initialize_file(save_file, num_iterations, nrows, ncols);
    }
    for (int i=0; i < num_iterations; i++) {
        if (SAVE_TO_FILE) {
            append_to_file(save_file, A, nrows, ncols);
        }
        if (DEBUG) {
            print_matrix(A, nrows, ncols);
        }
        evolve(A, A_old, nrows, ncols);
        memcpy(A_old, A, size_A*sizeof(bool));
    }
    delete[] A_old;
}


void evolve(bool* A, bool* A_old, int nrows, int ncols) {
    unsigned int num_alive_neighbors;
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            num_alive_neighbors = get_num_neighbors_alive(A_old, i, j, nrows, ncols);
            if (A_old[i*ncols + j]) {
                // current cell is alive
                if (num_alive_neighbors < 2 || num_alive_neighbors > 3) {
                    // cell doesn't have 2 or 3 neighbors, dies
                    A[i*ncols + j] = false;
                } else {
                    A[i*ncols + j] = true;
                }
            } else {
                // current cell is dead
                if (num_alive_neighbors == 3) {
                    // cell has 3 alive neighbors, can come alive
                    A[i*ncols + j] = true;
                } else {
                    A[i*ncols + j] = false;
                }
            }
        }
    }
}


// get the number of neighbors alive for a given cell (row/col)
unsigned int get_num_neighbors_alive(bool* A, int row, int col, int nrows, int ncols) {
    unsigned int num_alive_cells = 0;
    // number of rows and columns in A; will be used to help
    // loop around the grid if our cell is at a boundary.
    if (row == 0) {
        if (col == 0) {
            // can't check directly to the left or up

            // checking right
            num_alive_cells += (A[(row)*ncols + (col+1)] ? 1 : 0);
            //checking down
            num_alive_cells += (A[(row + 1)*ncols + col] ? 1 : 0);
            // checking down-right
            num_alive_cells += (A[(row + 1)*ncols + (col + 1)] ? 1 : 0);
            // checking loop-around cells
            // checking "left"
            num_alive_cells += (A[(row)*ncols + (ncols - 1)] ? 1 : 0);
            // checking "up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col)] ? 1 : 0);
            //checking "left-up" (bottom-right on grid)
            num_alive_cells += (A[(nrows - 1)*ncols + (ncols - 1)] ? 1 : 0);
            // checking "left-down"
            num_alive_cells += (A[(row + 1)*ncols + (ncols - 1)] ? 1 : 0);
            // checking "right-up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col + 1)] ? 1 : 0);
        } else if (col == ncols - 1) {
            // can't check anywhere directly to the right or up

            // checking left
            num_alive_cells += (A[(row)*ncols + (col - 1)] ? 1 : 0);
            num_alive_cells += (A[(row + 1)*ncols + (col)] ? 1 : 0);
            // checking down-left
            num_alive_cells += (A[(row + 1)*ncols + (col - 1)] ? 1 : 0);

            // checking "loop-around" cells
            // checking "up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col)] ? 1 : 0);
            // checking "right"
            num_alive_cells += (A[(row)*ncols + (0)] ? 1 : 0);
            // checking "right-up" (bottom-left)
            num_alive_cells += (A[(nrows - 1)*ncols + (0)] ? 1 : 0);
            // checking "left-up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col - 1)] ? 1 : 0);
            // checking "down-right"
            num_alive_cells += (A[(row + 1)*ncols + (0)] ? 1 : 0);
        } else {
            // col not at risk of going out of bounds
            // can't check directly up
            num_alive_cells += (A[(row)*ncols + (col + 1)] ? 1 : 0);
            num_alive_cells += (A[(row + 1)*ncols + (col + 1)] ? 1 : 0);
            num_alive_cells += (A[(row)*ncols + (col - 1)] ? 1 : 0);
            num_alive_cells += (A[(row + 1)*ncols + (col - 1)] ? 1 : 0);
            num_alive_cells += (A[(row + 1)*ncols + (col)] ? 1 : 0);

            // checking "up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col)] ? 1 : 0);
            // checking "right-up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col + 1)] ? 1 : 0);
            //checking "left-up"
            num_alive_cells += (A[(nrows - 1)*ncols + (col - 1)] ? 1 : 0);
        }
    } else if (row == nrows - 1) {
        if (col == 0) {
            // can't check directly to the left or down

            // checking up-right
            num_alive_cells += (A[(row - 1)*ncols + (col + 1)] ? 1 : 0);
            // checking up
            num_alive_cells += (A[(row - 1)*ncols + (col)] ? 1 : 0);
            num_alive_cells += (A[(row)*ncols + (col + 1)] ? 1 : 0);
            // checking "left"
            num_alive_cells += (A[(row)*ncols + (ncols - 1)] ? 1 : 0);
            // checking "down"
            num_alive_cells += (A[(0)*ncols + (col)] ? 1 : 0);
            // checking "down-right"
            num_alive_cells += (A[(0)*ncols + (col + 1)] ? 1 : 0);
            // checking "down-left" (top-right)
            num_alive_cells += (A[(0)*ncols + (ncols - 1)] ? 1 : 0);
            // checking "left-up"
            num_alive_cells += (A[(row - 1)*ncols + (ncols - 1)] ? 1 : 0);
        } else if (col == ncols - 1) {
            // can't check directly to the right or down
            num_alive_cells += (A[(row - 1)*ncols + (col - 1)] ? 1 : 0);
            num_alive_cells += (A[(row - 1)*ncols + (col)] ? 1 : 0);
            num_alive_cells += (A[(row)*ncols + (col - 1)] ? 1 : 0);

            // checking "right"
            num_alive_cells += (A[(row)*ncols + (0)] ? 1 : 0);
            // checking "down"
            num_alive_cells += (A[(0)*ncols + (col)] ? 1 : 0);
            // checking "down-right" (upper-left corner)
            num_alive_cells += (A[(0)*ncols + (0)] ? 1 : 0);
            // checking "down-left"
            num_alive_cells += (A[(0)*ncols + (col - 1)] ? 1 : 0);
            // checking "up-right"
            num_alive_cells += (A[(row - 1)*ncols + (0)] ? 1 : 0);

        } else {
            // col not at risk of going out of bounds
            // can't check down directly
            num_alive_cells += (A[(row)*ncols + (col + 1)] ? 1 : 0);
            num_alive_cells += (A[(row - 1)*ncols + (col + 1)] ? 1 : 0);
            num_alive_cells += (A[(row)*ncols + (col - 1)] ? 1 : 0);
            num_alive_cells += (A[(row - 1)*ncols + (col - 1)] ? 1 : 0);
            num_alive_cells += (A[(row - 1)*ncols + (col)] ? 1 : 0);

            // checking "down"
            num_alive_cells += (A[(0)*ncols + (col)] ? 1 : 0);
            // checking "down-left"
            num_alive_cells += (A[(0)*ncols + (col - 1)] ? 1 : 0);
            // checking "down-right"
            num_alive_cells += (A[(0)*ncols + (col + 1)] ? 1 : 0);

        }
    } else if (col == 0) {
        // row conditions have been checked already
        // can't check directly left
        num_alive_cells += (A[(row)*ncols + (col + 1)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col + 1)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col + 1)] ? 1 : 0);

        // checking "left"
        num_alive_cells += (A[(row)*ncols + (ncols - 1)] ? 1 : 0);
        // checking "down-left"
        num_alive_cells += (A[(row + 1)*ncols + (ncols - 1)] ? 1 : 0);
        // checking "up-left"
        num_alive_cells += (A[(row - 1)*ncols + (ncols - 1)] ? 1 : 0);
    } else if (col == ncols - 1) {
        // row conditions have been checked already
        // can't check right
        num_alive_cells += (A[(row)*ncols + (col - 1)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col - 1)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col - 1)] ? 1 : 0);

        // checking "right"
        num_alive_cells += (A[(row)*ncols + (0)] ? 1 : 0);
        // checking "down-right"
        num_alive_cells += (A[(row + 1)*ncols + (0)] ? 1 : 0);
        // checking "up-right"
        num_alive_cells += (A[(row - 1)*ncols + (0)] ? 1 : 0);
    } else {
        // no risk of going OOB in any direction
        num_alive_cells += (A[(row)*ncols + (col - 1)] ? 1 : 0);
        num_alive_cells += (A[(row)*ncols + (col + 1)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col - 1)] ? 1 : 0);
        num_alive_cells += (A[(row - 1)*ncols + (col + 1)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col - 1)] ? 1 : 0);
        num_alive_cells += (A[(row + 1)*ncols + (col + 1)] ? 1 : 0);
    }
    return num_alive_cells;
}
