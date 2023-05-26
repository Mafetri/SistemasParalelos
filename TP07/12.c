#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 1000

int main(int argc, char** argv) {
    int rank, i, j;
	int **matrix;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	matrix = malloc(MATRIX_SIZE * sizeof(int*));
	for (i = 0; i < MATRIX_SIZE; i++) {
		matrix[i] = malloc(MATRIX_SIZE * sizeof(int));
	}

	MPI_Datatype column_type;
	MPI_Type_vector(MATRIX_SIZE, 1, MATRIX_SIZE, MPI_INT, &column_type);
	MPI_Type_commit(&column_type);

    if (rank == 0) {
		for (i = 0; i < MATRIX_SIZE; i++) {
			for (j = 0; j < MATRIX_SIZE; j++) {
				matrix[i][j] = rank + 1;
			}
		}

		for (j = 0; j < MATRIX_SIZE; j++) {
			MPI_Send(&matrix[0][j], 1, column_type, 1, 0, MPI_COMM_WORLD);
		}
    } else if (rank == 1){
        for (j = 0; j < MATRIX_SIZE; j++) {
			MPI_Recv(&matrix[0][j], 1, column_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	}
    }

    MPI_Finalize();
    return 0;
}