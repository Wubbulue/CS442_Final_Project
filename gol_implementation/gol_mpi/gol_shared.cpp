#include <stdio.h>
#include <mpi.h>
#include <utils.hpp>
#include <cmath>
#include <cstring>

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

void update_local(bool *global_sim, bool *global_sim_old,int local_len,int sub_row,int sub_col,int n_dims){
    unsigned int num_alive_neighbors;
    int start_row = local_len*sub_row;
    int start_col = local_len*sub_col;
    for (int i = start_row; i < start_row+local_len; i++) {
        for (int j = start_col; j < start_col+local_len; j++) {
            num_alive_neighbors = get_num_neighbors_alive(global_sim_old, i, j, n_dims, n_dims);
            if (global_sim_old[i*n_dims + j]) {
                // current cell is alive
                if (num_alive_neighbors < 2 || num_alive_neighbors > 3) {
                    // cell doesn't have 2 or 3 neighbors, dies
                    global_sim[i*n_dims + j] = false;
                } else {
                    global_sim[i*n_dims + j] = true;
                }
            } else {
                // current cell is dead
                if (num_alive_neighbors == 3) {
                    // cell has 3 alive neighbors, can come alive
                    global_sim[i*n_dims + j] = true;
                } else {
                    global_sim[i*n_dims + j] = false;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    // parsing command-line args
    int n_iter, n_dims, seed;
    std::string file_path, save_file;
    bool file_io_flag;
    parse_args(argc, argv, n_iter, n_dims, file_path, file_io_flag, save_file, seed);

    MPI_Init(NULL, NULL);


    int rank_global;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_global);
    bool * global_sim;
    bool * global_sim_old;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 0;

    MPI_Win win_actual;
    int win_size = (rank == root)? n_dims * n_dims * sizeof(bool) : 0;
    MPI_Win_allocate_shared(win_size, 1, MPI_INFO_NULL,
            MPI_COMM_WORLD, &global_sim, &win_actual);

    if (rank != root) {
        int disp_unit;
        MPI_Aint other_size;
        MPI_Win_shared_query(win_actual, root, &other_size, &disp_unit, &global_sim);
    }

    MPI_Win win_old;
    MPI_Win_allocate_shared(win_size, 1, MPI_INFO_NULL,
            MPI_COMM_WORLD, &global_sim_old, &win_old);

    if (rank != root) {
        int disp_unit;
        MPI_Aint other_size;
        MPI_Win_shared_query(win_old, root, &other_size, &disp_unit, &global_sim_old);
    }


    int local_len = n_dims/std::sqrt(size);

    int sub_row,sub_col;
    switch (rank)
    {
    case 0:
        sub_row = 0;
        sub_col = 0;
        break;
    case 1:
        sub_row = 0;
        sub_col = 1;
        break;
    case 2:
        sub_row = 1;
        sub_col = 0;
        break;
    case 3:
        sub_row = 1;
        sub_col = 1;
        break;
    }


    if (file_path == "") {
        // no file given, random initialize
        if (rank == root) initialize_board_randomly(global_sim, n_dims, n_dims, seed);
    }
    else {
        if (rank == root) initilize_board_from_file(file_path, global_sim, n_dims, n_dims);
    }

    if(rank==0){
        std::memcpy(global_sim_old, global_sim, n_dims*n_dims);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int start_row = local_len*sub_row;
    int start_col = local_len*sub_col;

    // printf("Proc: %d, %d,%d, start_row: %d, start_col: %d\n",rank,sub_row,sub_col,start_row,start_col);

    double start = MPI_Wtime();
    for (int i=0; i < n_iter; i++) {
        if (file_io_flag) {
            if (rank == root) append_to_file(save_file, global_sim, n_dims, n_dims);
        }
        update_local(global_sim, global_sim_old, local_len, sub_row, sub_col, n_dims);
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank==0){
            std::memcpy(global_sim_old, global_sim, n_dims*n_dims);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    double end = MPI_Wtime();

    if(rank==root){
        printf("Running a GOL Simulation with %d rows, %d cols, and %d iterations took %f miliseconds\n",n_dims,n_dims,n_iter,(end-start)*1000);
        // print_matrix(global_sim,n_dims,n_dims);
    }

    MPI_Win_free(&win_actual);
    MPI_Win_free(&win_old);

    MPI_Finalize();
}
