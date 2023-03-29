#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	float a[5];

	*(a+5) = 6;

	printf("El valor de la pos 5 es: %f \n", a[5]);
}
