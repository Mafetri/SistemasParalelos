#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>
#define X 10000000

int main(void) {
    int i;
    float *m;
    float sum = 0;
    
    if (posix_memalign((void **)&m, 64, X * sizeof(float)) != 0) {
        return 1;
    }

    __m512 sum_vec = _mm512_setzero_ps();
    __m512 vector;
    
    for (i = 0; i < X; i++) {
        m[i] = i;
    }
    
    for (i = 0; i < X; i += 16) {
        vector = _mm512_load_ps(m + i);
        sum_vec = _mm512_add_ps(sum_vec, vector); 
    }
    
    float *sum_array;
    if (posix_memalign((void **)&sum_array, 64, 16 * sizeof(float)) != 0) {
        return 1;
    }
    _mm512_store_ps(sum_array, sum_vec);
    for (i = 0; i < 16; i++) {
        sum += sum_array[i];
    }
    printf("sum: %f \n", sum);

    return 1;
}

