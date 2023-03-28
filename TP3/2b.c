int main() {
   register int i, j, a;
   for (i = 0; i < 1000000; i++)
      for (j = 0; j < 1000; j++)
         a = i + j;
   return 0;
}
