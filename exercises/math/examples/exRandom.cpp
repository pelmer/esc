//  g++ -O2 -std=gnu++0x -msse4 -ftree-vectorize -ftree-vectorizer-verbose=1 -Iinclude examples/exRandom.cpp include/sfmt.cc
#include <x86intrin.h>

#include<random>
#include<iostream>

#include "sfmt.h"
#include "gaussian_ziggurat.h"

union V4 {
  __m128 v;
  float f[4];
};

union UI4 {
  __m128 v;
  __m128i si;
  unsigned int u[4];
  unsigned char c[16];
};



namespace mathSSE {

  class uniform_real_distribution {
  public:
    __m128 operator()(CRandomSFMT0 const & sfmt) const {
      float const fact = 1.f/2147483648.0f;
      __m128 const vfact = {fact,fact,fact,fact};
      return _mm_cvtepi32_ps(sfmt())*vfact;
    }
  };


  class normal_distribution {
    mutable __m128 val;
    mutable bool exist;
  public:
    normal_distribution() : exist(false){}


    __m128 operator()(CRandomSFMT0 const & sfmt) const {
      if (exist) {
	exist=false;
	return val;
      }
      __m128 u = uniform_real_distribution()(sfmt);
      __m128 v = uniform_real_distribution()(sfmt);
      __m128 s = u*u + v*v;

      exist=true;

    }
    

  };


};

int main() {

   std::mt19937 eng;
   std::mt19937 eng2;
   // std::ranlux_base_01 reng;
   std::ranlux48_base reng;
   std::uniform_int_distribution<int> ugen(1,50);
   std::uniform_real_distribution<double> rgen(-5.,5.);
   std::poisson_distribution<int> poiss(5.);
   std::normal_distribution<double> gauss(0.,1.);
   mathSSE::uniform_real_distribution urd128;

   for (int i=0; i<5; i++) {
      std::cout << eng() << std::endl;
      std::cout << eng2() << std::endl;
      std::cout << double(eng()-eng.min())/double(eng.max()-eng.min()) << std::endl;
      std::cout << ugen(eng) << std::endl;
      std::cout << rgen(reng) << std::endl;
      std::cout << poiss(reng) << std::endl;
      std::cout << gauss(reng) << std::endl;
      std::cout << gaussian_ziggurat(eng,1.) << std::endl;
      std::cout << std::endl;
    }


   CRandomSFMT0 sfmt(1234);
   V4 v;
   UI4 u;
   for (int a=0; a!=2; ++a) {
     for  (int i=0; i<5; i++) {
       u.si = sfmt();
       for  (int j=0; j<4; j++)
	 std::cout << u.u[j] << " ";
       std::cout << std::endl;
       v.v = _mm_cvtepi32_ps(u.si);
       for  (int j=0; j<4; j++)
	 std::cout << v.f[j] << " ";
       std::cout << std::endl;
       v.v = urd128(sfmt);
       for  (int j=0; j<4; j++)
	 std::cout << v.f[j] << " ";
       std::cout << std::endl;
     }
     std::cout << std::endl;
   }

   {
     // dice rolling...
     std::cout << " dice rolling..." << std::endl;
     UI4 six; six.si = _mm_set1_epi8(6);
     UI4 seven; seven.si = _mm_set1_epi8(7);
     
     
     long long iter=0;
     int dice[6] = {0};
     
     for ( int j=0; j!=60000; ++j) {   
       
       int mres =0;
       UI4 res; res.si= seven.si;
       __m128i mask; mask = _mm_setzero_si128 ();
       while (mres!=0xffff) {
	 ++iter;
	 u.si = sfmt();
	 u.si = _mm_and_si128(seven.si,u.si); // remove higher bits
	 __m128i m = _mm_cmplt_epi8(u.si,six.si);
	 res.si = _mm_blendv_epi8(res.si,u.si,_mm_andnot_si128(mask,m));
	 mask = _mm_or_si128(mask,m);
	 mres = _mm_movemask_epi8(mask);
       }
       
       //  for (int i=0;i!=16; ++i)
       //  std::cout << int(res.c[i]) << " ";
       // std::cout << std::endl;
       
       for (int i=0;i!=16; ++i)
	 ++dice[res.c[i]];
       
     }
     
     for (int i=0;i!=6; ++i)
       std::cout << dice[i] << " ";
   std::cout << ", iter tot "<< iter << std::endl;
   
   }


   {
     // dice rolling...
     std::cout << " dice rolling 2..." << std::endl;
     UI4 six; six.si = _mm_set1_epi8(6);
     UI4 seven; seven.si = _mm_set1_epi8(7);
     
     
     long long iter=0;
     int dice[6] = {0};

     UI4 res0; res0.si= seven.si;
     __m128i mask0; mask0 = _mm_setzero_si128 ();

     for ( int j=0; j!=60000; ++j) {   
       
       int mres =0;
       UI4 res = res0;
       __m128i mask = mask0;
       
       res0.si= seven.si;
       mask0 = _mm_setzero_si128 ();
       while (mres!=0xffff) {
	 ++iter;
	 u.si = sfmt();
	 u.si = _mm_and_si128(seven.si,u.si); // remove higher bits
	 __m128i m = _mm_cmplt_epi8(u.si,six.si);
	 res.si = _mm_blendv_epi8(res.si,u.si,_mm_andnot_si128(mask,m));
	 // move the rest in res0;
	 res0.si = _mm_blendv_epi8(res0.si,u.si,_mm_andnot_si128(mask0,_mm_and_si128(mask,m)));
	 mask0 = _mm_or_si128(mask0,_mm_and_si128(mask,m));

	 mask = _mm_or_si128(mask,m);
	 mres = _mm_movemask_epi8(mask);
       }
       
       //  for (int i=0;i!=16; ++i)
       //  std::cout << int(res.c[i]) << " ";
       // std::cout << std::endl;
       
       for (int i=0;i!=16; ++i)
	 ++dice[res.c[i]];
       
     }
     
     for (int i=0;i!=6; ++i)
       std::cout << dice[i] << " ";
   std::cout << ", iter tot "<< iter << std::endl;
   
   }


}
