

#include <emmintrin.h>
#include<cmath>

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



void approx(fasi & in) {
  fasi out;
  _mm_store_ss( &out.x, _mm_rsqrt_ss( _mm_load_ss( &in.x ) ) ); // compiles to movss, rsqrtss, movss
  print(out);
  fasi res = out; res.x *=res.x;
  print(res);
  out.x =  out.x * (1.5f - 0.5f * in.x * out.x * out.x); // One (more?) round of Newton's method
  print(out);
  out.x = in.x*out.x;
  print(out);
 
  _mm_store_ss( &out.x,_mm_rcp_ss( _mm_load_ss( &in.x ) ) );
  print(out);
  out.x = (out.x+out.x) - in.x*out.x*out.x;
  print(out);
} 


void accurate(fasi & in) {
  fasi out;
  out.x = std::sqrt(in.x);
  print(out);
  out.x = 1.f/out.x;
  print(out);
  out.x = 1.f/in.x;
  print(out);
}

int main() {
  fasi tmp; tmp.x=3.f;
  for (int i=0; i!=3;++i) {
    print(tmp);
    approx(tmp);
    std::cout << std::endl;
    accurate(tmp);
    incr(tmp);
    std::cout << std::endl;
    std::cout << std::endl;
  }
  return 0;
}
