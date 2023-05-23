#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
	int rank, comm_size, p_name_leng;
    char p_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Get_processor_name(p_name, &p_name_leng);

    printf("Rank: %d, Comm Size: %d, Node Name: %s!\n", rank, comm_size, p_name);
    MPI_Finalize();
    return 0;
}