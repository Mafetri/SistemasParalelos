#include <omp.h>
#include <stdio.h>
#include <time.h>

int primo(int n) {
    int i, res, mitad;
    i = 2;
    res = 1;
    mitad = n / 2;

    while(res != 0 && i <= mitad) {
        res = n % i;
        i++;
    }  
    return res;
}

int main (void) {
    int start = 0;
    int end = 100000;
    int i;
    int cant;

    #pragma omp parallel reduction(+:cant) 
    {
        int id = omp_get_thread_num();
        double time_start = omp_get_wtime();
        
        #pragma omp for schedule(dynamic, 10) nowait
        for(i = start; i <= end; i++) {
            if(primo(i)) cant++;
        }

        printf("%d termine despues de %f segundos \n", id, omp_get_wtime() - time_start);
    }    
    printf("Total de primos: %d \n", cant);
}
