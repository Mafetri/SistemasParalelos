#include <stdlib.h>

int main() {
   register unsigned int i, res = 0;
   for (i = 0; i < 500000000; i++)
      res += i*2;
   return 0;
}