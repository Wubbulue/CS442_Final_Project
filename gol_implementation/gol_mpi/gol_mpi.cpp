#include <mpi.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <chrono>
#include <cstring>

#include <utils.hpp>


struct NeighborRanks{
    int topleft;
    int above;
    int topright;
    int right;
    int bottomright;
    int bottom;
    int bottomleft;
    int left;
};

struct NeighborData{

    //Allocate heap memory
    NeighborData(int ncols, int nrows) : ncols(ncols), nrows(nrows) {
        s_above_row = new bool[ncols];
        s_below_row = new bool[ncols];
        s_right_col = new bool[nrows];
        s_left_col = new bool[nrows];

        r_above_row = new bool[ncols];
        r_below_row = new bool[ncols];
        r_right_col = new bool[nrows];
        r_left_col = new bool[nrows];
    }

    //Free heap memory
    ~NeighborData(){
        delete[] s_above_row;
        delete[] s_below_row;
        delete[] s_right_col;
        delete[] s_left_col;

        delete[] r_above_row;
        delete[] r_below_row;
        delete[] r_right_col;
        delete[] r_left_col;
    }

    //Send and receive neighboring data
    //local_sim: my local portion of the simulation
    //nRnks: ranks of my neighbors
    //cart_comm: cartesian grid communicator object
    void communicate(const bool *local_sim,const NeighborRanks &nRnks, MPI_Comm* cart_comm){

        //Copy rows to be sent
        std::memcpy(s_above_row,local_sim,ncols);
        const bool *last_row = local_sim+((nrows-1)*ncols);
        std::memcpy(s_below_row,last_row,ncols);

        //Copy cols to be sent
        for (int row = 0;row<nrows;row++){
            s_right_col[row] = local_sim[(ncols-1)+(row*ncols)];
            s_left_col[row] = local_sim[row*ncols];
        }

        //Copy corner cells
        s_top_left_cell = local_sim[0];
        s_top_right_cell = local_sim[ncols-1];
        s_bottom_left_cell = local_sim[(nrows-1)*ncols];
        s_bottom_right_cell = local_sim[(nrows-1)*ncols+(ncols-1)];

        //Send Rows and Cols
        MPI_Isend(s_above_row, ncols, MPI_C_BOOL, nRnks.above, above_row_tag, *cart_comm, reqs);
        MPI_Isend(s_below_row, ncols, MPI_C_BOOL, nRnks.bottom, below_row_tag, *cart_comm, reqs+1);
        MPI_Isend(s_right_col, nrows, MPI_C_BOOL, nRnks.right, right_col_tag, *cart_comm, reqs+2);
        MPI_Isend(s_left_col, nrows, MPI_C_BOOL, nRnks.left, left_col_tag, *cart_comm, reqs+3);

        //Send Corners
        MPI_Isend(&s_top_left_cell, 1, MPI_C_BOOL, nRnks.topleft, top_left_tag, *cart_comm, reqs+4);
        MPI_Isend(&s_top_right_cell, 1, MPI_C_BOOL, nRnks.topright, top_right_tag, *cart_comm, reqs+5);
        MPI_Isend(&s_bottom_left_cell, 1, MPI_C_BOOL, nRnks.bottomleft, bottom_left_tag, *cart_comm, reqs+6);
        MPI_Isend(&s_bottom_right_cell, 1, MPI_C_BOOL, nRnks.bottomright, bottom_right_tag, *cart_comm, reqs+7);

        //Receive Rows and Cols
        MPI_Irecv(r_above_row, ncols, MPI_C_BOOL, nRnks.above, below_row_tag, *cart_comm, reqs+8);
        MPI_Irecv(r_below_row, ncols, MPI_C_BOOL, nRnks.bottom, above_row_tag, *cart_comm, reqs+9);
        MPI_Irecv(r_right_col, nrows, MPI_C_BOOL, nRnks.right, left_col_tag, *cart_comm, reqs+10);
        MPI_Irecv(r_left_col, nrows, MPI_C_BOOL, nRnks.left, right_col_tag, *cart_comm, reqs+11);

        MPI_Irecv(&r_top_left_cell, 1, MPI_C_BOOL, nRnks.topleft, bottom_right_tag, *cart_comm, reqs+12);
        MPI_Irecv(&r_top_right_cell, 1, MPI_C_BOOL, nRnks.topright, bottom_left_tag, *cart_comm, reqs+13);
        MPI_Irecv(&r_bottom_left_cell, 1, MPI_C_BOOL, nRnks.bottomleft, top_right_tag, *cart_comm, reqs+14);
        MPI_Irecv(&r_bottom_right_cell, 1, MPI_C_BOOL, nRnks.bottomright, top_left_tag, *cart_comm, reqs+15);

        MPI_Waitall(16, reqs, MPI_STATUS_IGNORE);
    }

    //Prefix with s for send, r for receive
    bool *s_above_row;
    bool *s_below_row;
    bool *s_right_col;
    bool *s_left_col;
    bool s_top_left_cell,s_top_right_cell,s_bottom_right_cell,s_bottom_left_cell;

    bool *r_above_row;
    bool *r_below_row;
    bool *r_right_col;
    bool *r_left_col;
    bool r_top_left_cell,r_top_right_cell,r_bottom_right_cell,r_bottom_left_cell;

    int ncols,nrows;

    MPI_Request reqs[16];

    int above_row_tag = 1,
        below_row_tag = 2, 
        right_col_tag = 3, 
        left_col_tag = 4, 
        top_left_tag = 5,
        top_right_tag=6,
        bottom_right_tag = 7,
        bottom_left_tag=8;
};

// get the number of neighbors alive for a given cell (row/col)
//Note: row col here are relative to my local portion, not absolute
unsigned int get_num_neighbors_alive(bool* A, int row, int col, int nrows, int ncols,const NeighborData &nData){
    unsigned int num_alive_cells = 0;
    // loop around the grid if our cell is at a boundary.
    if (row == 0) {
        if (col == 0) {
            // can't check directly to the left or up

            // checking right
            num_alive_cells += A[(row)*ncols + (col+1)];
            //checking down
            num_alive_cells += A[(row + 1)*ncols + col];
            // checking down-right
            num_alive_cells += A[(row + 1)*ncols + (col + 1)];
            // checking loop-around cells
            // checking "left"
            num_alive_cells += nData.r_left_col[0];
            // checking "up"
            num_alive_cells += nData.r_above_row[0];
            //checking "left-up" (bottom-right on grid)
            num_alive_cells += nData.r_top_left_cell;
            // checking "left-down"
            num_alive_cells += nData.r_left_col[1];
            // checking "right-up"
            num_alive_cells += nData.r_above_row[1];
        } else if (col == ncols - 1) {
            // can't check anywhere directly to the right or up

            // checking left
            num_alive_cells += A[(row)*ncols + (col - 1)];
            num_alive_cells += A[(row + 1)*ncols + (col)];
            // checking down-left
            num_alive_cells += A[(row + 1)*ncols + (col - 1)];

            // checking "loop-around" cells
            // checking "up"
            num_alive_cells += nData.r_above_row[ncols-1];
            // checking "right"
            num_alive_cells += nData.r_right_col[0];
            // checking "right-up" (bottom-left)
            num_alive_cells += nData.r_top_right_cell;
            // checking "left-up"
            num_alive_cells += nData.r_above_row[ncols-2];
            // checking "down-right"
            num_alive_cells += nData.r_right_col[1];
        } else {
            // col not at risk of going out of bounds
            // can't check directly up
            num_alive_cells += A[(row)*ncols + (col + 1)];
            num_alive_cells += A[(row + 1)*ncols + (col + 1)];
            num_alive_cells += A[(row)*ncols + (col - 1)];
            num_alive_cells += A[(row + 1)*ncols + (col - 1)];
            num_alive_cells += A[(row + 1)*ncols + (col)];

            // checking "up"
            num_alive_cells += nData.r_above_row[col];
            // checking "right-up"
            num_alive_cells += nData.r_above_row[col+1];
            //checking "left-up"
            num_alive_cells += nData.r_above_row[col-1];
        }
    } else if (row == nrows - 1) {
        if (col == 0) {
            // can't check directly to the left or down

            // checking up-right
            num_alive_cells += A[(row - 1)*ncols + (col + 1)];
            // checking up
            num_alive_cells += A[(row - 1)*ncols + (col)];
            num_alive_cells += A[(row)*ncols + (col + 1)];
            // checking "left"
            num_alive_cells += nData.r_left_col[nrows-1];
            // checking "down"
            num_alive_cells += nData.r_below_row[0];
            // checking "down-right"
            num_alive_cells += nData.r_below_row[1];
            // checking "down-left" (top-right)
            num_alive_cells += nData.r_bottom_left_cell;
            // checking "left-up"
            num_alive_cells += nData.r_left_col[nrows-2];
        } else if (col == ncols - 1) {
            // can't check directly to the right or down
            num_alive_cells += A[(row - 1)*ncols + (col - 1)];
            num_alive_cells += A[(row - 1)*ncols + (col)];
            num_alive_cells += A[(row)*ncols + (col - 1)];

            // checking "right"
            num_alive_cells += nData.r_right_col[nrows-1];
            // checking "down"
            num_alive_cells += nData.r_below_row[ncols-1];
            // checking "down-right" (upper-left corner)
            num_alive_cells += nData.r_bottom_right_cell;
            // checking "down-left"
            num_alive_cells += nData.r_below_row[ncols-2];
            // checking "up-right"
            num_alive_cells += nData.r_right_col[nrows-2];

        } else {
            // col not at risk of going out of bounds
            // can't check down directly
            num_alive_cells += A[(row)*ncols + (col + 1)];
            num_alive_cells += A[(row - 1)*ncols + (col + 1)];
            num_alive_cells += A[(row)*ncols + (col - 1)];
            num_alive_cells += A[(row - 1)*ncols + (col - 1)];
            num_alive_cells += A[(row - 1)*ncols + (col)];

            // checking "down"
            num_alive_cells += nData.r_below_row[col];
            // checking "down-left"
            num_alive_cells += nData.r_below_row[col-1];
            // checking "down-right"
            num_alive_cells += nData.r_below_row[col+1];

        }
    } else if (col == 0) {
        // row conditions have been checked already
        // can't check directly left
        num_alive_cells += A[(row)*ncols + (col + 1)];
        num_alive_cells += A[(row + 1)*ncols + (col)];
        num_alive_cells += A[(row + 1)*ncols + (col + 1)];
        num_alive_cells += A[(row - 1)*ncols + (col)];
        num_alive_cells += A[(row - 1)*ncols + (col + 1)];

        // checking "left"
        num_alive_cells += nData.r_left_col[row];
        // checking "down-left"
        num_alive_cells += nData.r_left_col[row+1];
        // checking "up-left"
        num_alive_cells += nData.r_left_col[row-1];
    } else if (col == ncols - 1) {
        // row conditions have been checked already
        // can't check right
        num_alive_cells += A[(row)*ncols + (col - 1)];
        num_alive_cells += A[(row + 1)*ncols + (col)];
        num_alive_cells += A[(row + 1)*ncols + (col - 1)];
        num_alive_cells += A[(row - 1)*ncols + (col)];
        num_alive_cells += A[(row - 1)*ncols + (col - 1)];

        // checking "right"
        num_alive_cells += nData.r_right_col[row];
        // checking "down-right"
        num_alive_cells += nData.r_right_col[row+1];
        // checking "up-right"
        num_alive_cells += nData.r_right_col[row-1];
    } else {
        // no risk of going OOB in any direction
        num_alive_cells += A[(row)*ncols + (col - 1)];
        num_alive_cells += A[(row)*ncols + (col + 1)];
        num_alive_cells += A[(row - 1)*ncols + (col)];
        num_alive_cells += A[(row + 1)*ncols + (col)];
        num_alive_cells += A[(row - 1)*ncols + (col - 1)];
        num_alive_cells += A[(row - 1)*ncols + (col + 1)];
        num_alive_cells += A[(row + 1)*ncols + (col - 1)];
        num_alive_cells += A[(row + 1)*ncols + (col + 1)];
    }
    return num_alive_cells;

}

void evolve(bool* A, bool* A_old, int nrows, int ncols,const NeighborData &nData) {
    unsigned int num_alive_neighbors;
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            num_alive_neighbors = get_num_neighbors_alive(A_old, i, j, nrows, ncols,nData);
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

//Get the ranks of my neighbors within the cartesian grid
void getNeighborRanks(NeighborRanks* nRnks,  int my_coord[2], MPI_Comm* cart_comm){

    //indexed like col,row
    int coord_neighbor[2] = {my_coord[0],my_coord[1]};

    //Top Left
    {
        //Col and row minus 1
        coord_neighbor[0]--;
        coord_neighbor[1]--;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->topleft);
    }

    //Above
    {
        //Col plus 1
        coord_neighbor[0]++;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->above);
    }

    //Top Right
    {
        //Col plus 1
        coord_neighbor[0]++;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->topright);
    }

    //Right
    {
        //Row plus 1
        coord_neighbor[1]++;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->right);
    }

    //Bottom Right
    {
        //Row plus 1
        coord_neighbor[1]++;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->bottomright);
    }

    //Bottom 
    {
        //Col minus 1
        coord_neighbor[0]--;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->bottom);
    }

    //Bottom Left
    {
        //Col minus 1
        coord_neighbor[0]--;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->bottomleft);
    }

    //Left
    {
        //Row minus 1
        coord_neighbor[1]--;
        MPI_Cart_rank(*cart_comm, coord_neighbor, &nRnks->left);
    }

}

int main(int argc, char** argv) {
    // parsing command-line args
    int n_iter, n_dims, seed;
    std::string file_path, save_file;
    bool file_io_flag;
    parse_args(argc, argv, n_iter, n_dims, file_path, file_io_flag, save_file, seed);

    int dim[2], period[2], reorder;
    int coord[2], id;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // print out to check
        printf("n_iter: %d | n_dims: %d | seed: %d\n", n_iter, n_dims, seed);
        // cout required bc C++ doesn't know how to printf strings
        std::cout << "file_path: " << file_path << std::endl;
        std::cout << "file_io_flag: " << file_io_flag << std::endl;
        std::cout << "save_file: " << save_file << std::endl;
    }

    dim[0]=std::sqrt(size); dim[1]=std::sqrt(size);
    period[0]=1; period[1]=1;

    //Idk if this should be true or false
    reorder=1;

    MPI_Comm cart_comm;

    //indexed like col,row
    int my_coord[2];
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &cart_comm);
    MPI_Cart_coords(cart_comm, rank, 2, my_coord);
    // printf("Rank %d coordinates are %d %d\n", rank, my_coord[0], my_coord[1]);


    NeighborRanks neighborRanks;
    getNeighborRanks(&neighborRanks,my_coord,&cart_comm);

    // printf("Rank %d top right rank is %d\n",rank,neighborRanks.topright);



    //TODO: what if this isn't evenly distributed?
    const int ncols = n_dims/dim[0];
    const int nrows = n_dims/dim[1];

    bool *local_sim = new bool[ncols*nrows];
    bool *local_sim_old = new bool[ncols*nrows];
    
    int n = ncols;
    
    MPI_Datatype row_type;
    MPI_Type_vector(n, n, n_dims, MPI_CXX_BOOL, &row_type);
    MPI_Type_commit(&row_type);
    
    bool *A = new bool [n_dims*n_dims];
    int root = 0;

    if (file_path == "") {
        // no file given, random initialize
        if (rank == root) initialize_board_randomly(A, n_dims, n_dims, seed);
        scatter(size, rank, A, local_sim, root, n_dims, dim[0], n, row_type);
    }
    else {
        if (rank == root) initilize_board_from_file(file_path, A, n_dims, n_dims);
        scatter(size, rank, A, local_sim, root, n_dims, dim[0], n, row_type);
    }

    // printf("row: %d, col %d\n",my_coord[1],my_coord[0]);
    // print_matrix(local_sim,nrows,ncols);

    NeighborData nData(ncols,nrows);


    if(rank==root){
		initialize_file(save_file, n_iter,n_dims,n_dims);
    }


    double start = MPI_Wtime();
    std::memcpy(local_sim_old, local_sim, ncols*nrows*sizeof(bool));
    for (int i=0; i < n_iter; i++) {
        if (file_io_flag) {
            gather(size, rank, A, local_sim, root, n_dims, dim[0], n, row_type);
            if (rank == root) append_to_file(save_file, A, n_dims, n_dims);
        }
        nData.communicate(local_sim,neighborRanks, &cart_comm);
        evolve(local_sim, local_sim_old, nrows, ncols,nData);
        memcpy(local_sim_old, local_sim, ncols*nrows*sizeof(bool));
    }
    double end = MPI_Wtime();

    gather(size, rank, A, local_sim, root, n_dims, dim[0], n, row_type);

    //TODO: take max time like we learned in class here
    if(rank==0){
        printf("Running a GOL Simulation with %d rows, %d cols, and %d iterations took %f miliseconds\n",n_dims,n_dims,n_iter,(end-start)*1000);
        print_matrix(A,n_dims,n_dims);
    }

    MPI_Type_free(&row_type);

    // Finalize the MPI environment.
    MPI_Finalize();
    delete[] A;
    delete[] local_sim;
    delete[] local_sim_old;
}
