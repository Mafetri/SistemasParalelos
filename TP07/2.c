#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
	int rank, comm_size, processor;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_size(MPI_COMM_WORLD, &processor);

    printf("Rank: %d, Comm Size: %d, Node Name: %d!\n", rank, comm_size, processor);
    MPI_Finalize();
    return 0;
}