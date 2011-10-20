float poly1(float x, float a, float b, float c, float d) {
  return a + b*x + c*x*x + d*x*x*x;
}


float sum(float const * __restrict__ x, float const * __restrict__ y, unsigned long n) {
  float sum = 0;
  for (int i=0; i!=n; ++i)
    sum+=x[i]*y[i];
  return sum;
}

void prod(float const * __restrict__ x, float const * __restrict__ y, float * __restrict__ z, unsigned long n) {
  for (int i=0; i!=n; ++i)
    z[i]=x[i]*y[i];
}


void prod20(float const * __restrict__ x, float const * __restrict__ y, float * __restrict__ z) {
  for (int i=0; i!=20; ++i)
    z[i]=x[i]*y[i];
}

float sum20(float const * __restrict__ x, float const * __restrict__ y) {
  float sum = 0;
  for (int i=0; i!=20; ++i)
    sum+=x[i]*y[i];
  return sum;
}

struct V {
  float __attribute__ ((aligned(16))) v[20];
};

void proda(V const &  __restrict__ x, V const &  __restrict__ y, V &  __restrict__ z) {
  for (int i=0; i!=20; ++i)
    z.v[i]=x.v[i]*y.v[i];
}

V prodb(V const &  __restrict__ x, V const &  __restrict__ y) {
  V z;
  for (int i=0; i!=20; ++i)
    z.v[i]=x.v[i]*y.v[i];
  return z;
}


