#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

int main() {
   int i, elementos = 100000000;
   int *v;
   if (posix_memalign((void **)&v, 64, elementos * sizeof(int)) != 0) {
        return 1;
    }
    __m512i vector5 = _mm512_set1_epi32(5);
    __m512i vector;
    __m512i limite = _mm512_set1_epi32(50000000);
    __mmask16 mascara;

   //no vectorizar este for
   for (i = 0; i < elementos; i++)
      v[i] = i;

    for (i = 0; i < elementos; i += 16) {
        // Vector a analizar
        vector = _mm512_load_epi32(v + i);
        // Vector Mascara, tiene unos en las posiciones en la cual la posicion
        // del vector es mas grande que la posicion del vector de limite
        mascara = _mm512_cmpgt_epi32_mask(vector, limite);
        // Hace vector - vector5 en el caso que mascara en dicha posicion sea un 1
        // sino, retorna en dicha posicion lo que este en dicha posicion en el primer
        // vector de parametro
        vector = _mm512_mask_sub_epi32(vector, mascara, vector, vector5);
        // Lo mismo que antes pero se niega la mascara para sumar lo contrario
        vector = _mm512_mask_add_epi32(vector, ~mascara, vector, vector5);
        // Se alamacena dicho vector en dicha posicion del arreglo
        _mm512_store_epi32((__m512i*)(v + i), vector);
    }

   printf("elemento 0: %d\n",v[0]);
   printf("elemento 499999999: %d\n",v[49999999]);
   printf("elemento 500000000: %d\n",v[50000000]);
   printf("elemento 999999999: %d\n",v[99999999]);

   return 0;
}
