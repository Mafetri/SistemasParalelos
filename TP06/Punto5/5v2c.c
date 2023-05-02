#include <stdio.h>
#include <omp.h>

void main () {
    long num_steps = 800000000;
    double step;
    int i;
    int max_threads = 8;
    int array[max_threads][8];
    double x, pi, sum = 0.0;

    step = 1.0/(double) num_steps;

    #pragma omp parallel firstprivate(sum, x, i) num_threads(max_threads)
    {
        int id = omp_get_thread_num();
        int section_length = num_steps / omp_get_num_threads();
        int start = section_length * id;
        int end = section_length * (id+1);

        for (i = start; i < end; i++){
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }

        array[id][0] = sum;
    }

    for(int i = 0; i < max_threads; i++) {
        pi += step * array[i][0];
    }

    printf("%f\n",pi);
}
