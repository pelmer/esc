#include <emmintrin.h>
#include <cmath>
#include "icsiLog.h"

union fasi{
  int i;
  float x;
};


#include<iostream>
#include<iomanip>
#include<cstdio>
#include<cassert>

template<int N>
class SmallFloat {
public:
  SmallFloat(float imin, float imax) :
    rfact(imax), fact(1.f/rfact) {
    float const log2 = std::log(2.);
    fasi fmin; fmin.x = imin/imax;
    int emin = ((fmin.i >> 23) & 255) - 127;
    int ebits = log(-emin)/log2+1;
    n = N - ebits;
    assert(!(n<0)); 
    emax = 0x1 << (ebits);
    rmask = 0x1 << (23-n-1);
    dmask =  0x7FFFFF >> (23-n); 
  }

  int pack(float f) const {
    fasi tmp; tmp.x = f*fact;
    int log_2 = emax + ((tmp.i >> 23) & 255) -127;
    if (log_2<0) return 0;
    if (log_2>emax) return  0x7FFFFF >> (23-N);
    int rounding = (tmp.i & rmask) ? 1 : 0;
    int k = (tmp.i & 0x7FFFFF) >> (23-n); 
    k+=rounding; k += (log_2<<n);
    return k;
  }

  float unpack(int i) const  {
    int log_2 = (i>>n) - emax + 127;
    fasi tmp; tmp.i = log_2 <<23;
    tmp.i |= (i&dmask)<<(23-n);
    return tmp.x*rfact;
  }

public:
  float rfact;
  float fact;
  int n;
  int rmask;
  int emax;
  int dmask;
};


int main () {

  float const log2 = std::log(2.);
  float const offset = std::pow(2.,-23);


  fasi fmin; fmin.x = std::exp(-18);
  const int emin = ((fmin.i >> 23) & 255) - 127;//exponent
  std::cout << fmin.x << " " << emin << " " << log(-emin)/log2 << std::endl;
  float fact = 1.f/std::exp(9);

  int n = 3;
  int mask = 0x1 << (23-n-1);

  int i=0;
  SmallFloat<8> sf9(std::exp(-9), std::exp(9));
  SmallFloat<8> sf6(std::exp(-6), std::exp(6));
  std::cout << "sf 9 " << sf9.emax << " " << sf9.n << std::endl;
  std::cout << "sf 6 " << sf6.emax << " " << sf6.n << std::endl;

  for (double eta=-9; eta<9; eta+=0.125) { 
    float f = std::exp(eta);
    fasi tmp; tmp.x=f;
    int log_2 = ((tmp.i >> 23) & 255) - 127;//exponent
    tmp.i &= 0x7FFFFF; //mantissa
    float m = (tmp.i|0x800000)*offset-1.f;

    std::cout << std::log(f) << " " <<  log2*log_2 << " " <<  m+log2*log_2   << " " <<  m*(1.f-0.5f*m)+log2*log_2  
	      << " " << ln16(f) << " " << ln10(f) << " " << ln5(f) << std::endl;

    //
    tmp.x = f*fact;
    log_2 = 32 + ( ((tmp.i >> 23) & 255) -127);
    int rounding = (tmp.i & mask) ? 1 : 0;
    int k = (tmp.i & 0x7FFFFF) >> (23-n); 
    k+=rounding; k += (log_2<<n);
    // k |= (log_2<<n);
    int p9 = sf9.pack(f);
    int p6 = sf6.pack(f);
    std::cout << i << " " << k << " " << log_2 
	      << "  " << p9 << " " << std::log(sf9.unpack(p9))
 	      << " " << p6 << " " << std::log(sf6.unpack(p6)) << std::endl;
    ++i;
  }

  return 0;

}
