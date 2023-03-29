#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
	int rows = 3;
	int colms = 4;
	int* m[rows];
	for(int i = 0; i < rows; i++) {
		m[i] = (int*) malloc(colms* sizeof(int));
		for(int j = 0; j < colms; j++){
			m[i][j] = i+j;
			printf("m[%d][%d] = %d \n", i, j, m[i][j]);
		}
	}
	
	return 0;
}
