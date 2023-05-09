#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
	int i, j, A[1000];
	j = 5;
	#pragma omp parallel for firstprivate(j) 
	for (i = 1; i < 1001; i++) {
		j = 2 * i + 5;
		A[i-1] = 3 * j * j * j + 2 * j * j + 10 * j + 2;
	}

	printf("A[234] = %d \n", A[234]);
}