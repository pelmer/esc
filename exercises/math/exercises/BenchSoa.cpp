#include "Benchmark.h"

typedef Benchmark<float,4096> BM;

#include "AoSvsSoA.cc"

bool p=false;

float aosTest(int k, BM & bm) {
  aos::s=k+0.4f;
  aos::lsum(p);
  return (aos::m[k%1024]>bm.dummies[k]) ? aos::m[0] : aos::m[(BM::NVAL-k)%1024];
}

float parTest(int k, BM & bm) {
  aosP::s=k+0.4f;
  aosP::lsum(p);
  return (aosP::m[k%1024]>bm.dummies[k]) ? aosP::m[0] : aosP::m[(BM::NVAL-k)%1024];
}


float soa4Test(int k, BM & bm) {
  soa4::s=k+0.4f;
  soa4::soAsum(p);
  return (soa4::m[k%1024]>bm.dummies[k]) ? soa4::m[0] : soa4::m[(BM::NVAL-k)%1024];
}

float soa3Test(int k, BM & bm) {
  soa3::s=k+0.4f;
  soa3::soAsum(p);
  return (soa3::m[k%1024]>bm.dummies[k]) ? soa3::m[0] : soa3::m[(BM::NVAL-k)%1024];
}

float soaPTest(int k, BM & bm) {
  soaP::s=k+0.4f;
  soaP::soAsum(p);
  return (soaP::m[k%1024]>bm.dummies[k]) ? soaP::m[0] : soaP::m[(BM::NVAL-k)%1024];
}


#include<cstdlib>
#include<cstring>

int main(int argc) {
  if (argc<2) p=true;
  BM bm;
  bm.generate();

  aos::fill(aos::a,bm.values0,1024);
  aos::fill(aos::b,bm.values1,1024);
  aos::fill(aos::c,bm.values2,1024);
  aosP::fill(aosP::a,bm.values0,1024);
  aosP::fill(aosP::b,bm.values1,1024);
  aosP::fill(aosP::c,bm.values2,1024);
  memcpy(bm.values0,soa4::m1,4*4096);
  memcpy(bm.values1,soa4::m2,4*4096);
  memcpy(bm.values2,soa4::m3,4*4096);
  memcpy(bm.values0,soa3::m1,12*1096);
  memcpy(bm.values1,soa3::m2,12*1096);
  memcpy(bm.values2,soa3::m3,12*1096);
  memcpy(bm.values0,soaP::m1,4*4096);
  memcpy(bm.values1,soaP::m2,4*4096);
  memcpy(bm.values2,soaP::m3,4*4096);

  MARK(bm,aosTest);
  MARK(bm,parTest);
  MARK(bm,soa4Test);
  MARK(bm,soa3Test);
  MARK(bm,soaPTest);


  return 0;
}
