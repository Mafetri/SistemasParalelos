#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int rows = 3;
	int colms = 4;
	int* m[rows];
	int* y = (int*) malloc(colms * rows * sizeof(int));
	for(int i = 0; i < rows; i++) {
		m[i] = &y[i*colms];
		for(int j = 0; j < colms; j++){
			m[i][j] = i+j;
			printf("m[%d][%d] = %d \n", i, j, m[i][j]);
		}
	}
	return 0;
}
