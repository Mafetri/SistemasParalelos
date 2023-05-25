#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define VECTOR_SIZE 1000

int main(int argc, char *argv[]) {
    int *sendbuf;
	int local_sum, n, rank, comm_size;
    int total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	if(rank == 0) {
		sendbuf = (int*)malloc(VECTOR_SIZE * sizeof(int));
		for (int i = 0; i < VECTOR_SIZE; i++) {
			sendbuf[i] = i;
		}
	}

	n = VECTOR_SIZE / comm_size;
	int local_vector[n];

    MPI_Scatter(sendbuf, n, MPI_INT, local_vector, n, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < comm_size; i++) {
        if (rank == i) {
            printf("Proceso: %d: [", rank);
            for (int j = 0; j < n; j++) {
                printf("%d, ", local_vector[j]);
            }
            printf("] \n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    for (int i = 0; i < n; i++) {
        local_sum += local_vector[i];
    }
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(rank == 0) {
		printf("La suma total es: %d\n", total_sum);
	}

    MPI_Finalize();
    return 0;
}