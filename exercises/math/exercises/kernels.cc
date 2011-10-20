float poly1(float x, float a, float b, float c, float d) {
  return a + b*x + c*x*x + d*x*x*x;
}


float sum(float const * x, float const * y, unsigned long n) {
  float sum = 0;
  for (int i=0; i!=n; ++i)
    sum+=x[i]*y[i];
  return sum;
}

void prod(float const *  x, float const *  y, float *  z, unsigned long n) {
  for (int i=0; i!=n; ++i)
    z[i]=x[i]*y[i];
}


