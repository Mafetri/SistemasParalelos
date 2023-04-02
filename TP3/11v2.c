#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

int main() {
   int i, elementos = 1000005;
   int *v;

   if (posix_memalign((void **)&v, 64, elementos * sizeof(int)) != 0) {
        return 1;
    }

   // no vectorizar este for
   for (i = 0; i < elementos; i++)
      v[i] = rand() % 10000;

   printf("v[0]=%11d, ", v[0]);
   printf("v[11]=%11d, ", v[11]);
   printf("v[elementos-1]=%11d\n", v[elementos-1]);

   for (i = 0; i < elementos; i += 16) {
      // v[i]
      __m512i vector_vI = _mm512_load_epi32(v + i);
      // 2 * v[i]
      __m512i vector_vI_x2 = _mm512_mullo_epi32(vector_vI, _mm512_set1_epi32(2));
      // i * 10
      __m512i vector_i_x10 = _mm512_mullo_epi32(_mm512_set1_epi32(i), _mm512_set1_epi32(10));
      // 2 * v[i] < i * 10
      __mmask16 mascara = _mm512_cmpgt_epi32_mask(vector_i_x10, vector_vI_x2);

      // Operacion 1: v[i] * 2
      __m512i op1 = vector_vI_x2;

      // Operacion 2: (v[i] - 2) * -2
      // -2
      __m512i menos_2 = _mm512_set1_epi32(-2);
      // (v[i] - 2) * -2
      __m512i op2 = _mm512_mullo_epi32(_mm512_add_epi32(vector_vI, menos_2), menos_2);

      /*
      if(2 * v[i] < i * 10)
         v[i] = v[i] * 2;
      else
         v[i] = (v[i] - 2) * -2;
      */

      // Aplica la mascara al resultado correspondiente
      __m512i res = _mm512_mask_blend_epi32(mascara, op1, op2);
      // Guarda el vector resultante en la posicion analizada
      _mm512_store_epi32((__m512i*)(v+i), res);
   }

   printf("v[0]=%11d, ", v[0]);
   printf("v[11]=%11d, ", v[11]);
   printf("v[elementos-1]=%11d\n", v[elementos-1]);

   return 0;
}
