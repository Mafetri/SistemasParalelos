#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int size, rank, up, down, left, right;
    int dims[2] = {3, 5};
    int periods[2] = {1, 1};
    int coords[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    MPI_Cart_shift(cart_comm, 0, 1, &up, &down);
    MPI_Cart_shift(cart_comm, 1, 1, &left, &right);

    printf("Proceso %d: arriba = %d, abajo = %d, izq = %d, der = %d\n", rank, up, down, left, right);

    MPI_Finalize();
    return 0;
}