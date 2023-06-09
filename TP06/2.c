#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char **argv)
{
    int id;

    #pragma omp parallel private(id)
    {
        id = omp_get_thread_num();
        usleep(500);
        printf("Soy el hilo %d de %d hilos...\n", id, omp_get_num_threads());
    }

    printf("Somos: %d hilos...\n", omp_get_num_threads());
    
    return 0;
}