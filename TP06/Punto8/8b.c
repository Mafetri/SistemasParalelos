#include <omp.h>

int main()
{
    int i, j;

    #pragma omp parallel for schedule(dynamic, 10) private(j)
    for (i = 0; i < 10000000; i++) {
        if(i % 2 == 0) {
            for (j = 0; j < 100; j++);
        } else {
            for (j = 0; j < 1000; j++);
        }
    }
}