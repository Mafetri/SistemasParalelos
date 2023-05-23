#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define LENGTH 500000000

int main(int argc, char *argv[]) {
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Request recv_status = MPI_REQUEST_NULL;
    MPI_Request send_status = MPI_REQUEST_NULL;
    int *A;
    int *B;
    int i;
    int num_process;
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    int start_time = MPI_Wtime();
    A = malloc(sizeof(int) * LENGTH);
    B = malloc(sizeof(int) * LENGTH);
    for(i = 0; i < LENGTH; i++) {
        A[i] = i + rank;    
    }

    if(rank == 0) {
        MPI_Isend(A, LENGTH, MPI_INT, 1, 0, MPI_COMM_WORLD, &send_status);
        MPI_Irecv(B, LENGTH, MPI_INT, num_process-1, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_status);
    } else {
        MPI_Isend(A, LENGTH, MPI_INT, (rank+1)%num_process, 0, MPI_COMM_WORLD, &send_status);
        MPI_Irecv(B, LENGTH, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_status);
    }

    int X;
    for(i = 0; i < LENGTH; i++){
        X += A[i];
    }

    MPI_Wait(&recv_status, MPI_STATUS_IGNORE);
    for(i = 0; i < LENGTH; i++) {
        A[i] = X + B[i];
    }

    printf("%d tardó %f segundos. \n", rank, MPI_Wtime() - start_time);
    MPI_Finalize();
}