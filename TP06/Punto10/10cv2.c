#include <stdio.h>
#include <omp.h>
#define N 1000
int main (void) {
    int i, x[1000], y[1001], z[1000];

    #pragma omp parallel
    {
        #pragma omp for
        for (i = N - 2; i >= 0; i--) {
            y[i] = y[i] + z[i];
        }

        #pragma omp for
        for (i = N - 2; i >= 0; i--) {
            x[i] = x[i] + y[i+1];
        }
    }
    
    printf("%d, %d \n", x[468], y[876]);
}