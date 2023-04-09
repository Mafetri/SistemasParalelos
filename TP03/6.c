#include <stdlib.h>

int main() {
   int i, j, a;
   register int temp;
   for (i = 0; i < 1000; i++)
      for (j = 0; j < 1000000; j = j + 4) {
        temp = i + j;
        a = temp + 1;
        a = temp + 2;
        a = temp + 3;
      }
   return 0;
}