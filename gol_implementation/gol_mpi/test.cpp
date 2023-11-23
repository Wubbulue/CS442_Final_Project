#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <cmath>

int *createMatrix (int nrows, int ncols) {
    int *matrix;
    int h, i, j;

    matrix = new int [nrows*ncols];

    for (h=0; h<nrows*ncols; h++) {
        matrix[h] = h+1;
    }

    return matrix;
}

bool* createMatrix (int n) {
    bool *matrix;

    matrix = new bool [n*n];

    for (int h=0; h<n*n; h++) {
        matrix[h] = (bool)(h%3);
    }

    return matrix;
}

void printArray (int *row, int nElements) {
    int i;
    for (i=0; i<nElements; i++) {
        printf("%d ", row[i]);
    }
    printf("\n");
}

void printArray (bool *row, int nElements) {
    int i;
    for (i=0; i<nElements; i++) {
        printf("%d ", row[i]);
    }
    printf("\n");
}

void scatter(int num_procs, int id, int* A, int* B, int root, int global_n, int dim, int n, MPI_Datatype row_type)
{
    int tag = 0;
    
    MPI_Request recv_request;
    MPI_Irecv(&B[0], n * n, MPI_INT, root, tag, MPI_COMM_WORLD, &recv_request);

    if (id == root) { 
        MPI_Request* send_requests = new MPI_Request[num_procs];
	for (int i = 0; i < dim; i++)
	{
	    for (int j = 0; j < dim; j++)
	    {
		int index = (i*global_n + j) * n;
		int proc = i*dim + j;
		printf("proc: %d value: %d\n", proc, A[index]);
		MPI_Isend(&A[index], 1, row_type, proc, tag, MPI_COMM_WORLD, &(send_requests[proc]));
	    }
	}
        MPI_Waitall(num_procs, send_requests, MPI_STATUSES_IGNORE);
    }

    MPI_Wait(&recv_request, MPI_STATUSES_IGNORE);
    
    MPI_Barrier(MPI_COMM_WORLD);
}

void scatter(int num_procs, int id, bool* A, bool* B, int root, int global_n, int dim, int n, MPI_Datatype row_type)
{
    int tag = 0;
    
    MPI_Request recv_request;
    MPI_Irecv(&B[0], n * n, MPI_CXX_BOOL, root, tag, MPI_COMM_WORLD, &recv_request);

    if (id == root) { 
        MPI_Request* send_requests = new MPI_Request[num_procs];
	for (int i = 0; i < dim; i++)
	{
	    for (int j = 0; j < dim; j++)
	    {
		int index = (i*global_n + j) * n;
		int proc = i*dim + j;
		printf("proc: %d value: %d\n", proc, A[index]);
		MPI_Isend(&A[index], 1, row_type, proc, tag, MPI_COMM_WORLD, &(send_requests[proc]));
	    }
	}
        MPI_Waitall(num_procs, send_requests, MPI_STATUSES_IGNORE);
    }

    MPI_Wait(&recv_request, MPI_STATUSES_IGNORE);
    
    MPI_Barrier(MPI_COMM_WORLD);
}

void gather(int num_procs, int id, int* A, int* B, int root, int global_n, int dim, int n, MPI_Datatype row_type)
{
    int tag = 1;
    
    MPI_Request send_request;
    MPI_Isend(&B[0], n * n, MPI_INT, root, tag, MPI_COMM_WORLD, &send_request);

    if (id == root) { 
        MPI_Request* recv_requests = new MPI_Request[num_procs];
	for (int i = 0; i < dim; i++)
	{
	    for (int j = 0; j < dim; j++)
	    {
		int index = (i*global_n + j) * n;
		int proc = i*dim + j;
		printf("proc: %d index: %d\n", proc, index);
		MPI_Irecv(&A[index], 1, row_type, proc, tag, MPI_COMM_WORLD, &(recv_requests[proc]));
	    }
	}
        MPI_Waitall(num_procs, recv_requests, MPI_STATUSES_IGNORE);
    }

    MPI_Wait(&send_request, MPI_STATUSES_IGNORE);
    
    MPI_Barrier(MPI_COMM_WORLD);
}

void gather(int num_procs, int id, bool* A, bool* B, int root, int global_n, int dim, int n, MPI_Datatype row_type)
{
    int tag = 1;
    
    MPI_Request send_request;
    MPI_Isend(&B[0], n * n, MPI_CXX_BOOL, root, tag, MPI_COMM_WORLD, &send_request);

    if (id == root) { 
        MPI_Request* recv_requests = new MPI_Request[num_procs];
	for (int i = 0; i < dim; i++)
	{
	    for (int j = 0; j < dim; j++)
	    {
		int index = (i*global_n + j) * n;
		int proc = i*dim + j;
		printf("proc: %d index: %d\n", proc, index);
		MPI_Irecv(&A[index], 1, row_type, proc, tag, MPI_COMM_WORLD, &(recv_requests[proc]));
	    }
	}
        MPI_Waitall(num_procs, recv_requests, MPI_STATUSES_IGNORE);
    }

    MPI_Wait(&send_request, MPI_STATUSES_IGNORE);
    
    MPI_Barrier(MPI_COMM_WORLD);
}

int main (int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int num_procs, id;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // Get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // Get own ID

    
    int global = 4;
    int dim = (int) std::sqrt(num_procs);
    int n = global/dim;
    
        
    MPI_Datatype row_type;
    MPI_Type_vector(n, n, global, MPI_CXX_BOOL, &row_type);
    MPI_Type_commit(&row_type);
    
    //int *A;
    bool *A;
    if (id == 0) {
        //A = createMatrix(global, global); // Master process creates matrix
        A = createMatrix(global); // Master process creates matrix
        printf("Initial matrix:\n");
        printArray(A, global*global);
    }

    //int *B = new int [n * n]; // submatrix will be n by n
    bool *B = new bool [n * n]; // submatrix will be n by n

    scatter(num_procs, id, A, B, 0, global, dim, n, row_type);  

    printf("Process %d received elements: ", id);
    printArray(B, n * n);
    
    if (id == 0) {
        for (int h=0; h<global*global; h++) {
            A[h] = false;
        }
        printf("Cleaned matrix:\n");
        printArray(A, global*global);
    }
    
    gather(num_procs, id, A, B, 0, global, dim, n, row_type); 
    
    if (id == 0) {
        printf("Final matrix:\n");
        printArray(A, global*global);
    }
    
    MPI_Type_free(&row_type);
    
    MPI_Finalize();

    return 0;
}
