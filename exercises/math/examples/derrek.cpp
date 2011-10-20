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


template<typename T, int SP>
inline void vSplit(T x, T & x_high, T &  x_low) __attribute__ ((always_inline));

template<typename T, int SP>
inline void vSplit(T x, T & x_high, T &  x_low) {
  const unsigned int C = ( 1 << SP ) + 1;
  T a = C * x;
  T b = x - a;
  x_high = a + b;
  x_low = x - x_high;
}

template <typename T> struct  SHIFT_POW{};

template <> struct  SHIFT_POW<float>{ enum {value=12}; /* 24/2 for single precision */ };
template <> struct  SHIFT_POW<double>{ enum {value = 27}; /* 53/2 for double precision */ };


template<typename T>
inline void dMultiply(T x, T y, T & r1, T & r2) __attribute__ ((always_inline));

template<typename T>
inline void dMultiply(T x, T y, T & r1, T & r2) {
  T x_high, x_low, y_high, y_low;
  vSplit<T,SHIFT_POW<T>::value>(x, x_high, x_low);
  vSplit<T,SHIFT_POW<T>::value>(y, y_high, y_low);
  r1 = x * y;
  T a = -r1 + x_high * y_high;
  T b =   a + x_high * y_low;
  T c =   b + x_low  * y_high;
    r2 =  c + x_low  * y_low;
}

template<typename T>
inline void dsquare(T x, T & r1, T & r2) __attribute__ ((always_inline));

template<typename T>
inline void dsquare(T x, T & r1, T & r2) {
  T x_high, x_low;
  vSplit<T,SHIFT_POW<T>::value>(x, x_high, x_low);
  r1 = x * x;
  T a = -r1 + x_high * x_high;
  T b =   a + (x_high * x_low);
  T c =   b + (x_high * x_low);
  r2 =  c + x_low  * x_low;
}

float dMultiply(float x, float y) {
  float r1, r2;
  dMultiply(x,y,r1,r2);
  return r1+r2;
}

double ddMultiply(float x, float y) {
  float r1, r2;
  dMultiply(x,y,r1,r2);
  return double(r1)+double(r2);
}


float prod(float x, float y) {
  return x*y;
}

int main() {
  fasi tmp1; tmp1.x=1.e-5f/3.f;
  fasi tmp2; tmp2.x=1.e5f/6.f;
  fasi res; 
  res.x = prod(tmp1.x,tmp2.x);
  print(res);
  fasi r1, r2;
  dMultiply(tmp1.x,tmp2.x,r1.x,r2.x);
  print (r1); print (r2);
  res.x =  dMultiply(tmp1.x,tmp2.x);
  print(res);  
  double bha = ddMultiply(tmp1.x,tmp2.x);
  printf("%e %a\n", bha, bha);
  bha = double(tmp1.x)*double(tmp2.x);
  printf("%e %a\n\n", bha, bha);

  float me = 0.511e-3f;
  float p = 50.f;

  float e = std::sqrt(p*p+me*me);
  printf("%e %a\n", e, e);
  double de = std::sqrt(double(p*p)+double(me*me));
  printf("%e %a\n", de, de);
  double dme0 =  std::sqrt(de*de-double(e*e));
  printf("%e %a\n", dme0, dme0);
  double dme1 =  std::sqrt((de-e)*(de+e));
  printf("%e %a\n", dme1, dme1);
  float fme1 =  std::sqrt((float(de)-e)*(float(de)+e));
  printf("%e %a\n", fme1, fme1);
   
  //dsquare(p,r1,r2);
  //dsquare(me,r1,r2);

}
