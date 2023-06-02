#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    FILE *f;
    char *s, *res;
    int rows, cols, steps, n, i, j;

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
    int rank, comm_size;
    MPI_Comm grid;
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
    char **old;
    old = malloc(total_size_my_rows * sizeof (char*));
    for (i = 0; i < total_size_my_rows; i++) {
        old[i] = (char *) malloc(total_size_my_colms * sizeof (char));
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
    
    // Envias a la derecha e izquierda la columna correspondiente
    int left_rank;
    int right_rank;
    MPI_Cart_shift(grid, 1, 1, &left_rank, &right_rank);
    MPI_Datatype column_type;
    MPI_Type_vector(size_my_rows, 1, total_size_my_colms, MPI_CHAR, &column_type);
    MPI_Type_commit(&column_type);
    MPI_Request send_request;
    MPI_Request send_request2;
    MPI_Request recv_request_left;
    MPI_Request recv_request_right;
    MPI_Isend(&old[1][1], 1, column_type, left_rank, 0, grid, &send_request);
    MPI_Isend(&old[1][size_my_colms], 1, column_type, right_rank, 1, grid, &send_request2);    
    MPI_Irecv(&old[1][total_size_my_colms-1], 1, column_type, right_rank, 0, grid, &recv_request_right);
    MPI_Irecv(&old[1][0], 1, column_type, left_rank, 1, grid, &recv_request_left);

    // Harias tu calculo interno
    for(i = 1; i < size_my_rows - 1; i++) {
        for(j = 1; j < size_my_colms - 1; j++){
            // Tocas a next step
        }
    }

    // Esperas recibir las columnas derecha e izquierda
    MPI_Wait(&recv_request_left, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_request_right, MPI_STATUS_IGNORE);

    // Envias las filas superiores e inferiores
    int down_rank;
    int up_rank;
    MPI_Cart_shift(grid, 0, 1, &up_rank, &down_rank);
    MPI_Datatype row_type;
    MPI_Request recv_request_up = MPI_REQUEST_NULL;
    MPI_Request recv_request_down = MPI_REQUEST_NULL;
    MPI_Type_vector(total_size_my_colms, 1, 1, MPI_CHAR, &row_type);
    MPI_Type_commit(&row_type);
    MPI_Isend(&old[1][0], 1, row_type, up_rank, 0, grid, &send_request);
    MPI_Isend(&old[size_my_rows][0], 1, row_type, down_rank, 1, grid, &send_request2);    
    MPI_Irecv(&old[0][0], 1, row_type, up_rank, 1, grid, &recv_request_up);
    MPI_Irecv(&old[size_my_rows+1][0], 1, row_type, down_rank, 0, grid, &recv_request_down);

    // Calculas los de la derecha e izquierda

    // Esperas recibir las filas superiores e inferiores
    MPI_Wait(&recv_request_left, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_request_right, MPI_STATUS_IGNORE);

    // Calculas las filas superiores e inferiores


    // Finaliza el ciclo
    
    MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < comm_size; i++) {
        if (rank == i) {
            printf("Soy: %d, derecha: %d izquierda: %d \n", rank, right_rank, left_rank);
            for(int k = 0; k < total_size_my_rows; k++) {
                for(int l = 0; l < total_size_my_colms; l++) {
                    printf("%d, ",old[k][l]);
                }
                printf("\n");
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    
    /*
        char filename[50];
        sprintf(filename, "subgrid3_%d_%d.out", coords[0], coords[1]);
        FILE *output = fopen(filename, "w");
        for(int k = 0; k < total_size_my_rows; k++) {
            for(int l = 0; l < total_size_my_colms; l++) {
                fprintf(output, "%d, ",old[k][l]);
            }
            fprintf(output, "\n");
        }
        fprintf(output, "\n");
    */

    /* Columnas con sendrecv
    MPI_Wait(&recv_request_left, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&old[1][1], 1, column_type, left_rank, 0,
                &old[1][total_size_my_colms - 1], 1, column_type, right_rank, 0,
                grid, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&old[1][size_my_colms], 1, column_type, right_rank, 1,
             &old[1][0], 1, column_type, left_rank, 1,
             grid, MPI_STATUS_IGNORE);
    */

    /* Anterior forma de imprimir derecha e izquierda
    printf("%d rank: %d, recibi de %d y %d \n", size_my_colms, rank, left_rank, right_rank);
    printf("L   R\n");
    for(j = 0; j < size_my_rows; j++){
        printf("%d    ", left_column[j]);
        printf("%d \n", right_column[j]);
    }
    */


    // Envias la fila superior e inferior
    // Calculas tus costados (derecha e izquierda)
    // Esperas recibir las filas superiores e inferiores
    // Calculas tus filas superiores e inferiores
    // Reinicias el ciclo

    /*
    for (j = 0; j < MATRIX_SIZE; j++) {
		MPI_Recv(&matrix[0][j], 1, column_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // WAIT
    // Calculas con respesto a lo recivido

    MPI_Datatype row;
    MPI_Type_commit(&row);

    MPI_Type_vector(size_my_colms + 2, 1, 0, MPI_Datatype old_type, MPI_Datatype *newtype)


    
    */
    

    /*
    //Copias para hacer el mundo toroidal
    for (j = 1; j < ncols - 1; j++) { //Copia las columnas
        old[0][j] = old[nrows - 2][j];
        old[nrows - 1][j] = old[1][j];
    }

    for (i = 1; i < nrows - 1; i++) { //Copia las filas
        old[i][0] = old[i][ncols - 2];
        old[i][ncols - 1] = old[i][1];
    }

    //Copia las 4 esquinas
    old[0][0] = old[nrows - 2][ncols - 2];
    old[nrows - 1][0] = old[1][ncols - 2];
    old[0][ncols - 1] = old[nrows - 2][1];
    old[nrows - 1][ncols - 1] = old[1][1];

    //CALCULAR EL SIGUIENTE ESTADO DE MUNDO
    char **aux;
    char **nextStep; //Representa el estado siguiente del mundo
    nextStep = malloc(nrows * sizeof (char*));
    aux = malloc(nrows * sizeof (char*));

    for (i = 0; i < nrows; i++)
        nextStep[i] = (char *) malloc((cols + 2) * sizeof (char));
    int c;
    int cant1;
    for (c = 0; c < steps; c++) {        
        //Se recorre la matriz y se calcula el valor de su estado siguiente en la matriz nextStep
        for (i = 1; i <= rows; i++) {
            for (j = 1; j <= cols; j++) {
                //Suma las celdas vecinas para saber cuantas están vivas
                cant1 = old[i - 1][j - 1] + old[i - 1][j] + old[i - 1][j + 1] + old[i][j - 1] + old[i][j + 1] + old[i + 1][j - 1] + old[i + 1][j] + old[i + 1][j + 1];

                if (old[i][j] == 1) //si tiene 2 o 3 vecinas vivas, sigue viva
                    nextStep[i][j] = ((cant1 == 2 || cant1 == 3)) ? 1 : 0;
                else
                    //Si está muerta y tiene 3 vecinas vivas revive
                    nextStep[i][j] = (cant1 == 3) ? 1 : 0;
            }
        }

        //Copia los lados correspondientes en los lados auxiliares
        for (j = 1; j < ncols - 1; j++) { //Copia columnas
            nextStep[0][j] = nextStep[nrows - 2][j];
            nextStep[nrows - 1][j] = nextStep[1][j];
        }

        for (i = 1; i < nrows - 1; i++) {//Copia filas
            nextStep[i][0] = nextStep[i][ncols - 2];
            nextStep[i][ncols - 1] = nextStep[i][1];
        }
        //Copia las esquinas
        nextStep[0][0] = nextStep[nrows - 2][ncols - 2];
        nextStep[nrows - 1][0] = nextStep[1][ncols - 2];
        nextStep[0][ncols - 1] = nextStep[nrows - 2][1];
        nextStep[nrows - 1][ncols - 1] = nextStep[1][1];

        //Intercambio de punteros, para realizar el siguiente paso con los nuevos estados calculados   
        aux = old;
        old = nextStep;
        nextStep = aux;
    }

    //IMPRESIÓN DE LA CABECERA Y DEL ÚLTIMO ESTADO LUEGO DE EJECUTAR LOS STEPS
    printf("cols %d \nrows %d \nsteps %d \n", cols, rows, steps);
    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            printf("%c", old[i][j] == 1 ? 'O' : '.');
        }
        printf("\n");
    }

    */
    MPI_Finalize();
    return 0;
}