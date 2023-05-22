#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0) {
		int msg = 1234;
		MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else {
		int msg_recv;
		MPI_Recv(&msg_recv, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d recibe: %d \n", rank, msg_recv);
		MPI_Send(&msg_recv, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}