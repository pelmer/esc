#include <x86intrin.h>
#include <cstdio>

union Vec4F {
  __m128 vec;
  float arr[4];
  Vec4F(){}
  Vec4F(__m128 v) : vec(v){}
  float operator[](int i) const { return arr[i];}
};

void print(__m128 vec) {
  Vec4F v(vec);
  printf("(%e %e %e %e)\n", v[0],v[1],v[2],v[3]);

}


int main() {

  __m128 tvec({1.f,2.f,3.f,4.f});
  Vec4F v1(tvec);
  print(v1.vec);

  __m128 v = _mm_set1_ps(1.f);
  __m128 m1 = _mm_set_ps(4.f,3.f,2.f,1.f);
  __m128 m2 = _mm_set_ps(-4.f,-3.f,-2.f,-1.f);
  __m128 m3 = _mm_set_ps(4.f,3.f,-2.f,-1.f);



//  printf_s("m1: 0x%08x, 0x%08x, 0x%08x, 0x%08x\n",
//        m1.m128_u32[3], m1.m128_u32[2], m1.m128_u32[1], m1.m128_u32[0]);
  
  print(m1);
  print(_mm_dp_ps(v, m1, 0xff));
  print(_mm_dp_ps(v, m1, 0x2f));
  print(_mm_dp_ps(v, m1, 0xf8));

  print(_mm_or_ps(_mm_dp_ps(v, m1, 0xf1),_mm_dp_ps(v, m2, 0xf2)) );
  print(_mm_blendv_ps(m1,m2,_mm_cmpgt_ps(m1,m2)));

  return 0;
}
