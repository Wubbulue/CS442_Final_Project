#ifndef GOL_H
#define GOL_H
// perform 1 iteration of the Game of Life
// at start A == A_old, will modify A using A_old
// A[i][j]=true means alive, false dead
void evolve(bool* A, bool* A_old, int nrows, int ncols);
// get the number of neighbors alive for a given cell (row/col)
unsigned int get_num_neighbors_alive(bool* A, int row, int col, int nrows, int ncols);
// run will evolve, copy data to A_old, and save matrix (if SAVE_TO_FILE flag), then repeat.
// if DEBUG then print matrix every iteration
void run(bool* A, int num_iterations, int nrows, int ncols, bool SAVE_TO_FILE, bool DEBUG);
#endif
