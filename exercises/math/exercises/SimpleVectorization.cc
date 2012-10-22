float v0[1024];
float v1[1024];
float v2[1024];
float v3[1024];


void muladd() {
  for(int i=0; i!=1024; ++i)
    v0[i]= v1[i]+v2[i]*v3[i];
}

float innerProduct() {
   float s=0;
   for (int i=0; i!=1024; i++) 
      s+= v1[i]*v2[i];
   return s;
}


void condAssign() {
  for(int i=0; i!=1024; ++i)
    if (v2[i]>v1[i]) v0[i]=v2[i]*v3[i];
}

void condAssign2() {
  for(int i=0; i!=1024; ++i)
    v0[i] = (v2[i]>v1[i]) ? v2[i]*v3[i] : v0[i];
}


void condAlgo() {
  for(int i=0; i!=1024; ++i) {
    if (v2[i]>0.37f) v0[i]=v2[i]*v3[i];
    else v0[i]=v2[i]/v3[i];
  }
}


#include<algorithm>

inline
float t0(float xx, float yy) {
  if (yy>xx) return xx/yy;
  return yy/xx;
}


inline
float t1(float xx, float yy) {
  if (yy>xx) std::swap(yy,xx);
  return yy/xx;
}

inline
float t2(float xx, float yy) {
  return std::min(yy,xx)/std::max(yy,xx);
}

void vt0() {
  for(int i=0; i!=1024; ++i)
    v1[i]=t0(v2[i],v3[i]);
}


void vt1() {
  for(int i=0; i!=1024; ++i)
    v1[i]=t1(v2[i],v3[i]);
}


void vt2() {
  for(int i=0; i!=1024; ++i)
    v1[i]=t2(v2[i],v3[i]);
}
