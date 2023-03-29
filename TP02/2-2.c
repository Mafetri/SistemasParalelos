#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int col = 3;
	int rows = 4;
	float a[col][rows];

	for(int i = 0; i < col; i++) {
		for(int j = 0; j < rows; j++) {
			printf("Elemento [%d][%d] -0x%x \n", i, j, &a[i][j]);
		}
	}
}
