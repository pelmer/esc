
#include<cmath>

#ifdef __SSE3__
#include <mmintrin.h>
#include <pmmintrin.h>

void nosubnormal() {
//  _mm_setcsr (_mm_getcsr () | 0x8040);

  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
}

void subnormal() {
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
}
#else
void nosubnormal() {}
void subnormal() {}
#endif

union fasi{
  int i;
  float x;
};


#include<iostream>
#include<iomanip>
#include<cstdio>


void incr(fasi & f) {
  f.i++;
}

void print(fasi const & f) {
 printf("%e %a %x\n",  f.x,  f.x,  f.i);
}

void play(fasi tmp) {
  fasi tmp2; tmp2.x = 2.f*tmp.x;
  fasi tmp3; tmp3.x = std::sqrt(tmp.x);

  print(tmp);
  print(tmp2);
  print(tmp3);

}


int main() {
  fasi tmp; tmp.x=0;
  for (int i=0; i!=3;++i) {
    incr(tmp);
    play(tmp);
    nosubnormal();
    play(tmp);
    subnormal();
    play(tmp);
    printf("\n");
  }
  return 0;
}
