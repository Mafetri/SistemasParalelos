#include <omp.h>
#include <unistd.h>

int main() {
    int i, j;

    #pragma omp parallel private(j) 
    {
        int t = omp_get_thread_num();
        if(t > 10) sleep(1);

        #pragma omp for schedule(guided, 2) private(j)
        for (i = 0; i < 10000000; i++) {
            for (j = 0; j < 100; j++);
        }
    }
}