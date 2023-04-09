#include <stdlib.h>
#define X 50000
#define Y 50000

int main() {
   int i, j;
   char **m;
   m = malloc(X * sizeof(char*));
   for (i = 0; i < X; i++)
      m[i] = malloc(Y * sizeof(char));

   for(i = 0; i < X; i++){
      for(j = 0; j < Y; j++) {
         m[i][j] = j % 2;
      }
   }

   return 0;
}
