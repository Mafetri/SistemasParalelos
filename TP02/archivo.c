#include <stdio.h>

int main(){
	int x = 10;
	int *p;
	p = &x;
	printf("Direccion de x: %p \n", &x);
	printf("Direccion almacenada en *p: %d \n", p);
	printf("Valor de *p: %d \n", *p);

	return 0;
}
