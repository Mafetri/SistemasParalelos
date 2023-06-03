#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    FILE *f;
    char *s, *res;
    int rows, cols, steps, n, i, j, k;
    int rank, comm_size, down_rank, up_rank, left_rank, right_rank;

    //Controla que se haya ingresado un argumento en la llamada (el archivo con el  patrón de entrada) 
    if (argc != 2) {
        printf("Error: Debe indicar el nombre del archivo de entrada\n");
        return 1;
    }

    //Lectura del encabezado del archivo que contiene el patrón de celdas
    f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error al intentar abrir el archivo.\n");
        return 1;
    }

    n = fscanf(f, "cols %d\nrows %d\nsteps %d\n", &cols, &rows, &steps);

    if (n != 3) {
        printf("Error: formato de archivo incorrecto\n");
        return 1;
    }

    MPI_Init (&argc, &argv);
    // MPI Variables
    MPI_Comm grid;
    MPI_Request send_request;
    MPI_Request send_request2;
    MPI_Request recv_request_left;
    MPI_Request recv_request_right;
    MPI_Datatype column_type;
    MPI_Datatype row_type;
    MPI_Request recv_request_up = MPI_REQUEST_NULL;
    MPI_Request recv_request_down = MPI_REQUEST_NULL;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if(comm_size % 2 != 0) {
        printf("pelotudo");
        return 1;
    }

    //Construction of grid
    int dims[] = {(comm_size / 2), 2};
    int ndims = 2;
    int periods[] = {1, 1};
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &grid);

    // Coords of the process
    int coords[2];
    MPI_Cart_coords(grid, rank, ndims, coords);

    // Limits of the block to be computed
    int size_my_rows = rows / (comm_size / 2);
    int start_my_rows = coords[0] * size_my_rows;
    int end_my_rows = start_my_rows + size_my_rows;

    int size_my_colms = cols / 2;
    int start_my_colms = coords[1] * size_my_colms;
    int end_my_colms = start_my_colms + size_my_colms;

    // Total matrix to save the neighbours columns and rows
    int total_size_my_rows = size_my_rows + 2;
    int total_size_my_colms = size_my_colms + 2;
    
    //Se reserva memoria dinámica para la matriz de celdas, representada por el arreglo de punteros "old"	
    char *old[total_size_my_rows];
    char *old_temp = malloc(total_size_my_rows * total_size_my_colms * sizeof(char));
    for (i = 0; i < total_size_my_rows; i++) {
        old[i] = &old_temp[i * total_size_my_colms];
    }

    //Inicializa elementos de la matriz "old" con 0 o 1 segun el patron de entrada 
    i = 0;
    s = malloc(cols);
    res = fgets(s, cols, f);

    // Se inicializa la matriz con ceros
    for(int z = 0; z < total_size_my_rows; z++) {
        for(int j = 0; j < total_size_my_colms; j++) {
            old[z][j] = 0;
        }
    }

    // Se llena la matriz con 0s o 1s segun corresponda
    while (i <= rows && res != NULL) {
        for (j = 0; j < strlen(s) - 1; j++) {
            if(i >= start_my_rows && i < end_my_rows && j >= start_my_colms && j < end_my_colms) {
                old[(i-start_my_rows)+1][(j-start_my_colms)+1] = (s[j] == '.') ? 0 : 1;
            }
        }
        res = fgets(s, cols, f);
        i++;
    };

    fclose(f); //Se cierra el archivo 
    free(s); //Se libera la memoria utilizada para recorrer el archivo
    
    // Execution of the life game
    char *nextStep[total_size_my_rows];
    char *nextStep_temp = malloc(total_size_my_rows * total_size_my_colms * sizeof(char));
    for (i = 0; i < total_size_my_rows; i++) {
        nextStep[i] = &nextStep_temp[i * total_size_my_colms];
        for(j=0; j < total_size_my_colms; j++){
            nextStep[i][j] = 0;
        }
    }

    int c;
    int cant1;
    for (c = 0; c < steps; c++){
        // Envias a la derecha e izquierda la columna correspondiente
        MPI_Cart_shift(grid, 1, 1, &left_rank, &right_rank);
        MPI_Type_vector(size_my_rows, 1, total_size_my_colms, MPI_CHAR, &column_type);
        MPI_Type_commit(&column_type);
        MPI_Isend(&old[1][1], 1, column_type, left_rank, 0, grid, &send_request);
        MPI_Isend(&old[1][size_my_colms], 1, column_type, right_rank, 1, grid, &send_request2);    
        MPI_Irecv(&old[1][total_size_my_colms-1], 1, column_type, right_rank, 0, grid, &recv_request_right);
        MPI_Irecv(&old[1][0], 1, column_type, left_rank, 1, grid, &recv_request_left);

        // Harias tu calculo interno (desde [2][2] a [total_size_my_rows-3][total_size_my_colms-3])
        for (i = 2; i < size_my_rows; i++) {
            for (j = 2; j < size_my_colms; j++) {
                // Fila superior
                cant1 = old[i-1][j-1] + old[i-1][j] + old[i-1][j+1];
                // Fila actual
                cant1 += old[i][j-1] + old[i][j+1];
                // Fila inferior
                cant1 += old[i+1][j-1] + old[i+1][j] + old[i+1][j+1];

                if (old[i][j] == 1) {
                    // Si tiene 2 o 3 vecinas vivas, sigue viva
                    nextStep[i][j] = ((cant1 == 2 || cant1 == 3)) ? 1 : 0;
                } else {
                    //Si está muerta y tiene 3 vecinas vivas revive
                    nextStep[i][j] = (cant1 == 3) ? 1 : 0;
                }
            }
        }

        // Esperas recibir las columnas derecha e izquierda
        MPI_Wait(&recv_request_left, MPI_STATUS_IGNORE);
        MPI_Wait(&recv_request_right, MPI_STATUS_IGNORE);

        // Envias las filas superiores e inferiores
        MPI_Cart_shift(grid, 0, 1, &up_rank, &down_rank);
        MPI_Type_vector(total_size_my_colms, 1, 1, MPI_CHAR, &row_type);
        MPI_Type_commit(&row_type);
        MPI_Isend(&old[1][0], 1, row_type, up_rank, 0, grid, &send_request);
        MPI_Isend(&old[size_my_rows][0], 1, row_type, down_rank, 1, grid, &send_request2);    
        MPI_Irecv(&old[0][0], 1, row_type, up_rank, 1, grid, &recv_request_up);
        MPI_Irecv(&old[size_my_rows+1][0], 1, row_type, down_rank, 0, grid, &recv_request_down);

        // Calculas los de la derecha e izquierda
        // Harias tu calculo de las columnas exteriores (desde [2][2] a [total_size_my_rows-3][2])
        for (i = 2; i < size_my_rows; i++) {
            for(k = 0; k < 2; k++) {
                // Primera y ultima columna (sin las esquinas) 
                if(k == 0) {
                    j = 1;
                } else {
                    j = size_my_colms;
                }

                // Fila superior
                cant1 = old[i-1][j-1] + old[i-1][j] + old[i-1][j+1];
                // Fila actual
                cant1 += old[i][j-1] + old[i][j+1];
                // Fila inferior
                cant1 += old[i+1][j-1] + old[i+1][j] + old[i+1][j+1];

                if (old[i][j] == 1) {
                    // Si tiene 2 o 3 vecinas vivas, sigue viva
                    nextStep[i][j] = ((cant1 == 2 || cant1 == 3)) ? 1 : 0;
                } else {
                    //Si está muerta y tiene 3 vecinas vivas revive
                    nextStep[i][j] = (cant1 == 3) ? 1 : 0;
                }
            }
        }

        // Esperas recibir las filas superiores e inferiores
        MPI_Wait(&recv_request_up, MPI_STATUS_IGNORE);
        MPI_Wait(&recv_request_down, MPI_STATUS_IGNORE);

        if(rank == 0 && c == 0){
            printf("Soy: %d, arriba: %d abajo: %d \n", rank, up_rank, down_rank);
            for(int k = 0; k < total_size_my_rows; k++) {
                for(int l = 0; l < total_size_my_colms; l++) {
                    printf("%d, ",old[k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }

        // Calculas las filas superiores e inferiores
        // Harias tu calculo desde [1][1] a [1][total_size_my_colms-3]
        // y de [total_size_my_rows-3][1] a [total_size_my_rows-3][total_size_my_colms-3]
        for (k = 0; k < 2; k++) {
            // Primera y ultima fila
            if(k == 0) {
                i = 1;
            } else {
                i = size_my_rows;
            }

            for (j = 1; j <= size_my_colms; j++) {
                // Fila superior
                cant1 = old[i-1][j-1] + old[i-1][j] + old[i-1][j+1];
                // Fila actual
                cant1 += old[i][j-1] + old[i][j+1];
                // Fila inferior
                cant1 += old[i+1][j-1] + old[i+1][j] + old[i+1][j+1];

                if (old[i][j] == 1) {
                    // Si tiene 2 o 3 vecinas vivas, sigue viva
                    nextStep[i][j] = ((cant1 == 2 || cant1 == 3)) ? 1 : 0;
                } else {
                    //Si está muerta y tiene 3 vecinas vivas revive
                    nextStep[i][j] = (cant1 == 3) ? 1 : 0;
                }
            }
        }

        // Guarda en old el next step
        for(i = 0; i < total_size_my_rows; i++){
            for(j = 0; j < total_size_my_colms; j++){
                old[i][j] = nextStep[i][j];
            }
        }
    }

    char filename[50];
    sprintf(filename, "subgrid_%d_%d.out", coords[0], coords[1]);
    FILE *output = fopen(filename, "w");
    for(int k = 1; k <= size_my_rows; k++) {
        for(int l = 1; l <= size_my_colms; l++) {
            fprintf(output, "%d, ",old[k][l]);
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
    
    MPI_Finalize();
    return 0;
}