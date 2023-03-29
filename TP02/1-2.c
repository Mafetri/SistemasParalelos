#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
   int a, b;
   int *x, *y;

   a = 10;
   b = 20;
   x = &a;
   y = &b;
   x = y;
   *x = 5;
   
   printf("a: %d, b: %d \n", a, b);
   return 0;
}
