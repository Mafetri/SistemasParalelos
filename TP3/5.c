#include <stdlib.h>

int main() {
   int i, j, a;
   for (i = 0; i < 1000; i++) {
      for (j = 0; j < 1000000; j = j + 4) {
         a = i + j;
         a = i + j + 1;
         a = i + j + 2;
         a = i + j + 3;
      }
   }
   return 0;
}