#include<cmath>
#include <x86intrin.h>

float v0[1024];
float v1[1024];


inline
float isqrtf_est( float x ) {
  return _mm_cvtss_f32( _mm_rsqrt_ss( _mm_set_ss( x ) ) );
}


void v() {
  for(int i=0; i!=1024; ++i)
    v0[i] = isqrtf_est(v1[i]);
}


float v(float x) {
  return __builtin_rsqrtf(x);
}
