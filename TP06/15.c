#include <stdlib.h>
#define N 200000000

void vector_add(int *A, int *B, int *C, int n) {
    int i;
    for (i = 0; i < n; i++)
        C[i] = A[i] + B[i];
}

void rec_vector_add(int *A, int *B, int *C, int n) {
	if(n <= 1) {
		vector_add(A, B, C, n);
	} else {
		int n2 = n / 2;
		rec_vector_add(A, B, C, n2);
		rec_vector_add(A+n2, B+n2, C+n2, n-n2);
	}
}

int main() {
	int *a = malloc(N * sizeof(int));
	int *b = malloc(N * sizeof(int));
	int *c = malloc(N * sizeof(int));
	rec_vector_add(a, b, c, N);
}