#include <stdio.h>

int main() {
   int num;
   int *p1;
   int **p2;

   num = 123;
   p1 = &num;
   p2 = &p1;

   *p1 = num - 23;
   **p2 = *p1 * 2;

   printf("Valor de num = %d\n", num);

   return 0;
}
