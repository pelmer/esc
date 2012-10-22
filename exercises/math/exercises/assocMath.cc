#include <cmath>

float mul1(float a, float x, float y) {
   return a*x + a*y;
}


float div1(float a, float x, float y) {
   return a/x/y;
}

float div2(float a, float x, float y) {
   return a/(x*y);
}


float v0[1024];
float v1[1024];
float v2[1024];
float v3[1024];

float summ() {
  float s=0;
  for (int i=0; i!=1024;++i)
    s +=v1[i]*v2[i];
  return s;
}
void vdiv() {
  for (int i=0; i!=1024;++i)
    v0[i]=v1[i]/v2[i]/v3[i];
}


// extern "C"
double sqrt0(double a, double x, double y) {
  return a*(sqrt(x)*sqrt(y));
}


float sqrt1(float a, float x, float y) {
  return a*(std::sqrt(x)*std::sqrt(y));
}



float rsqrt1(float a, float x, float y) {
   return a/std::sqrt(x)/std::sqrt(y);
}

float rsqrt2(float a, float x, float y) {
   return a/sqrtf(x*y);
}


void vrsqrt() {
  for (int i=0; i!=1024;++i)
    v0[i]=v1[i]/std::sqrt(v2[i])/std::sqrt(v3[i]);
}


void rot(float x, float y, float & w, float & z) {
  w = std::sin(x) -std::cos(y);
  z = std::cos(x) +std::sin(y);
}

float poly1(float x, float a, float b, float c, float d) {
  return a + b*x + c*x*x + d*x*x*x;
}

float polyN1(float x, float const * a, int N) {
  float s=0;
  for (int j=0; j!=N; ++j) {
    s += a[j]*std::pow(x,float(j));
  }
  return s;
}

float polyN2(float x, float const * a, int N) {
  float s=0;
  float p=1.f;
  for (int j=0; j!=N; ++j) {
    s += a[j]*p;
    p*=x;
  }
  return s;
}
