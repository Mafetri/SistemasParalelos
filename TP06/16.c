#include <stdio.h>
#define LIMIT 2000000

double region (long start, long end, double step) {
    double sum = 0.0, x;
    long i;
    double parte1, parte2;

    if (end - start > LIMIT) {
        #pragma omp task shared(parte1)
        {
            parte1 = region(start, (end+start)/2, step);
        }

        #pragma omp task shared(parte2)
        {
            parte2 = region((start+end)/2, end, step);
        }
        
        #pragma omp taskwait
        sum = parte1 + parte2;
    } else {
        for (i = start; i < end; i++){
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }

    return sum;
}

void main () {
    long num_steps = 800000000;
    double sum, pi = 0.0;
    double step = 1.0/(double) num_steps;

    #pragma omp parallel
    {
        #pragma omp single
        {
            sum = region(0, num_steps, step);
        }
    } 

    pi = step * sum;
    printf("%f\n",pi);
}
