#include <stdlib.h>
#include <stdio.h>

#define X 10000000
int main(void){
	int i;
	float *m;
	float sum;
	m = malloc(X * sizeof(float*));
	for(i = 0; i < X; i++)
		m[i] = i;
	
	for(i = 0; i < X; i++)
		sum += m[i];

	printf("sum: %f \n", sum);

	return 1;
}
