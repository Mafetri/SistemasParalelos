#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0) {
		int msg1 = 1234;
		MPI_Send(&msg1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		
		char msg2[] = "hola";
		MPI_Send(&msg2, 5, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
	} else if (rank == 1){
		int msg_recv;
		MPI_Recv(&msg_recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Recibe: %d \n", msg_recv);

		char msg2_recv[4];
		MPI_Recv(&msg2_recv, 5, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Recibe: %s \n", msg2_recv);
	}

	MPI_Finalize();
}