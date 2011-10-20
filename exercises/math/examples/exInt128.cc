#include <emmintrin.h>
#include <iostream>

union W128_T {
  __m128i si;
  unsigned int u[4];
} __attribute__ ((aligned (16))) ;

/** 128-bit data type */
typedef union W128_T w128_t;


int main() {

  w128_t r[5];

  r[0].si = _mm_xor_si128(r[0].si,r[0].si);
  std::cout << r[0].u[0] << std::endl;

  return 0;

}
