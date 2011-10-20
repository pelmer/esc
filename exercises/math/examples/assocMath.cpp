// c++ -O2 -fassociative-math -funsafe-math-optimizations -freciprocal-math -fno-math-errno -c assocMath.cpp
//  c++ -O2 -fassociative-math -fno-signed-zeros -fno-trapping-math -freciprocal-math -fno-math-errno -ffinite-math-only  -c assocMath.cpp
#include<cmath>

#ifdef __SSE3__
#include <mmintrin.h>
#include <pmmintrin.h>

//__attribute__ ((__target__ ("recip")));

void nosubnormal() {
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

float const one = 1.f;
float const two = 2.f;
float const four = 4.f;
float const minusOne = -1.f;
float const zero = 0.f;
float const eps = 1.e-25f;

float once(float x) { return one*x;}
float twice(float x) { return 2*x;}
float twicef(float x) { return two*x;}
float fourt(float x) { return four*x;}

float sqr1(float x) { return std::pow(x,2);}
float sqr2(float x) { return std::pow(x,2.f);}

float rsqr1(float x) { return one/sqrt(x);}
float rsqr2(float x) { return one/sqrtf(x);}
float rsqr3(float x) { return one/std::sqrt(x);}
float rsqr4(float x) { return std::sqrt(one/x);}
float rsqr3r(float x) __attribute__ ((__target__ ("recip")));
float rsqr4r(float x) __attribute__ ((__target__ ("recip")));
float rsqr3r(float x) { return one/std::sqrt(x);}
float rsqr4r(float x) { return std::sqrt(one/x);}
float rsqr3f(float x) __attribute__ ((__optimize__ ("fast-math")));
float rsqr3f(float x) { return one/std::sqrt(x);}
float rsqr4f(float x) __attribute__ ((__optimize__ ("fast-math")));
float rsqr4f(float x) { return std::sqrt(one/x);}

float conv(float x) {
  x+=3.14;
  return pow(x,2);
}


float noconv(float x) {
  x+=3.14f;
  return std::pow(x,2.f);
}


float kk(float & w) {
  w = w + w;
  float y = w + one;
  float z = y - w;
  y = z - one;
  return y;
}

float kloop() {
  float y=0;
  float w = one;
  do  {
    w = w + w;
    y = w + one;
    float z = y - w;
    y = z - one;
  } while (minusOne + std::abs(y) < zero);
  /*.. now W is just big enough that |((W+1)-W)-1| >= 1 ...*/
  return w;
}


float ksum(float const * x, int N) {
  float s = x[0];
  float c = 0;
  for (int j=1;  j!=N; ++j) {
    float y = x[j] - c;
    float t = s + y;
    c = (t - s) - y;
    s = t;
  }
  return s;
}

float pluszero(float x) {
  return x + zero;
}
float prodzero(float x) {
  return x*zero;
}
float divzero(float x) {
  return zero/x;
}


float simpleassoc(float x) {
  return x + eps -x;
}

float multiassoc1(float x,float y) {
  float a = x + one + y;
  float b = x + two + y;
  return a/b;
}

float multiassoc2(float x,float y) {
  float a = x + (one + y);
  float b = x + (two + y);
  return a/b;
}

float multiassoc3(float x,float y) {
  float a = one + (x + y);
  float b = two + (x + y);
  return a/b;
}

float poly1(float x, float a, float b, float c, float d) {
  return a + b*x + c*x*x + d*x*x*x;
}

float poly2(float x, float a, float b, float c, float d) {
  return a + b*x + c*std::pow(x,2) + d*std::pow(x,3);
}

float poly3(float x, float const * a, int N) {
  float s=0;
  for (int j=0; j!=N; ++j) {
    s += a[j]*std::pow(x,j);
  }
  return s;
}

float poly4(float x, float const * a, int N) {
  float s=0;
  float p=1.f;
  for (int j=0; j!=N; ++j) {
    s += a[j]*p;
    p*=x;
  }
  return s;
}

float sum(float const * a) {
  float s=0;
  for (int j=0; j!=256; ++j) {
    s += a[j];
  }
  return s;
}


float quad(float x, float y) {
  return x*x - y*y;
}

float quadop(float x, float y) {
  return (x-y)*(x+y);
}


inline float quadi(float x, float y) __attribute__ ((always_inline));
inline float quadi(float x, float y)  {
  return x*x - y*y;
 }


float quadp(float x, float y) __attribute__ ((pure));
float quadp(float x, float y)  {
  return x*x - y*y;
 }

float useq(float x, float y, float w, float z) {
  float a = w + quad(x,y);
  float b = z + quad(x,y);
   return a/b;
}

float useqp(float x, float y, float w, float z) {
  float a = w + quadp(x,y);
  float b = z + quadp(x,y);
  return a/b;
}

float useqi(float x, float y, float w, float z) {
  float a = w + quadi(x,y);
  float b = z + quadi(x,y);
  return a/b;
}


float sc1(float x, float y) {
  return std::sin(x)/std::cos(y);
}

void rot(float x, float y, float & w, float & z) {
  w = std::sin(x) -std::cos(y);
  z = std::cos(x) +std::sin(y);
}


float exp0(float x) {
  return std::exp(x) - std::exp(x);
}

float exp1(float x, float y) {
  float a = one + std::exp(x + y);
  float b = two + std::exp(x + y);
   return a/b;
}


float exp2(float x, float y) {
  float a = one*std::exp(x);
  float b = two*std::exp(y);
   return a/b;
}

float exp3(float x) {
  float a = one*std::exp(x);
  float b = two*std::exp(-x);
   return a*b;
}

float sqrt0(float x) {
  return std::sqrt(x);
}

float sqrt1(float x, float y) {
  float a = one + std::sqrt(x + y);
  float b = two + std::sqrt(x + y);
   return a/b;
}


float sqrt2(float x, float y) {
  float a = one*std::sqrt(x);
  float b = two*std::sqrt(y);
   return a/b;
}

float sqrt3(float x) {
  float a = one*std::sqrt(x);
  float b = two*std::sqrt(one/x);
   return a*b;
}

float sqrt4(float x) {
  float a = one*std::sqrt(x);
  float b = two/std::sqrt(x);
  return a*b;
}



float sqrt5(float x, float y) {
  float p = std::sqrt(x*x+y*y);
  float a = x/p/p;
  float b = y/p/p;
  return a+b;

}


float  div0(float x, float y) {
  return one/x/y;
}

float  div1(float x, float y, float z) {
  float a = one + x/y;
  float b = two + z/y;
   return a+b;
}

float  div2(float x, float y, float z) {
  float a = (z + x)/y;
  float b = (z - x)/y;
   return a+b;
}

float  div3(float x, float y) {
  float a = one/x/y;
  float b = two/x/y;
   return a+b;
}

float  div3s(float x, float y) {
  float a = one/x/y;
  float b = two/y/x;
   return a+b;
}

float  div4(float x) {
  float a = one/x;
  float b = two*x;
   return a*b;
}

float  div5(float x) {
  return one*two*x/x;
}

float  div5s(float x) {
  return x/x*two*one;
}


float prod1(float x, float y) {
  float a = x*y;
  float b = y*x;
  return a+b;
}


float next(float x) {
  union {
    int i;
    float x;
  } tmp;

  tmp.x=x;
  tmp.i++;
  return tmp.x;

}
