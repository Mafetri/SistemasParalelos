#include <stdlib.h>
#define X 1000000

int main() {
   int i, j, **m;
   m = malloc(900 * sizeof(int*));
   for (i = 0; i < 900; i++)
      m[i] = malloc(X * sizeof(int));

   for(i = 0; i < 300; i++) {
    for(j = 0; j < X; j++) {
        m[i][j] = 0;
    }
   }
   for(i = 300; i < 600; i++) {
    for(j = 0; j < X; j++) {
        m[i][j] = 1;
    }
   }
   for(i = 600; i < 900; i++) {
    for(j = 0; j < X; j++) {
        m[i][j] = 2;
    }
   }
   return 0;
}
