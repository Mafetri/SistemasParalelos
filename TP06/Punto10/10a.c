#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int i, j, A[1000];
	j = 5;
	for (i = 0; i < 1000; i++) {
		j += 2;
		A[i] = 3 * j * j * j + 2 * j * j + 10 * j + 2;
	}
	printf("A[234] = %d\n", A[234]);
}