#include <stdlib.h>
#include <stdio.h>
#define N 200000000

void vector_add(int *A, int *B, int *C, int n) {
    int i;
    for (i = 0; i < n; i++)
        C[i] = A[i] + B[i];
}

void rec_vector_add(int *A, int *B, int *C, int n) {
	int n2 = n / 2;

	if(n > 1000000){
		#pragma omp task
		{
			rec_vector_add(A, B, C, n2);
			rec_vector_add(A+n2, B+n2, C+n2, n-n2);
		}
	} else {
		vector_add(A, B, C, n);
	}
}

int main() {
	int *a = malloc(N * sizeof(int));
	int *b = malloc(N * sizeof(int));
	int *c = malloc(N * sizeof(int));

	#pragma omp parallel
	{
		#pragma omp single
		{
			rec_vector_add(a, b, c, N);
		}
	}
}