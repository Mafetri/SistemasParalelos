#include <stdio.h>
#include <omp.h>

void main () {
    long num_steps = 800000000;
    double step;
    int i;
    int threads_used;    
    double x, pi, sum = 0.0;

    step = 1.0/(double) num_steps;
    omp_set_dynamic(1);
    #pragma omp parallel firstprivate(x, i) reduction(+:sum)
    {
        int id = omp_get_thread_num();
        int section_length = num_steps / omp_get_num_threads();
        int start = section_length * id;
        int end = section_length * (id+1);
        double sum_local;

        for (i = start; i < end; i++){
            x = (i + 0.5) * step;
            sum_local = sum_local + 4.0 / (1.0 + x * x);
        }

        sum += sum_local;
    }

    pi = step * sum;
    printf("%f\n", pi);
}