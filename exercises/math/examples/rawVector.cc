#include<cmath>
#include <x86intrin.h>

typedef float vector __attribute__ ((vector_size (16)));
typedef int   ivector __attribute__ ((vector_size (16)));


vector add (vector a, vector b) {
  return a+b;
}

void sub( vector & a, vector b) {
  a-=b;
}


vector psca(vector a, float b) {
  vector vb = {b,b,b,b};
  return a*vb;
}

vector neg(vector a) {
  return -a;
}

/*
ivector conv(vector a) {
  ivector i =a;
  return i;
}
*/

vector work(vector a, vector b) {
  vector q = {1.f,3.f,-4.f,2.f};
  //  return std::sqrt(a/q) +b;
  return a/q + b;
}

__m128 prod(__m128 a, __m128 b) {
  return a*b;
}


__m128i vand(__m128i a, __m128i b) {
  return a&b;
}

