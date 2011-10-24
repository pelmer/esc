/*
 *
 *
 *
 *
 */
#include "Benchmark.h"

typedef Benchmark<float,4096> BM;


float atest(int k, BM & bm) {
  for (int i=1; i!=BM::NVAL; ++i) {
     // the kernel to test
    bm.res[i] = std::atan2(bm.values0[i],bm.values2[i]);
   }
  return (bm.res[k]>bm.dummies[k]) ? bm.res[0] : bm.res[BM::NVAL-k];
}

float dtest(int k, BM & bm) {
  for (int i=1; i!=BM::NVAL; ++i) {
     // the kernel to test
    //     bm.res[i] = std::atan2(bm.values0[i],bm.values2[i]);
     bm.res[i] = bm.values0[i]/bm.values2[i];
   }
  return (bm.res[k]>bm.dummies[k]) ? bm.res[0] : bm.res[BM::NVAL-k];
}

float mtest(int k, BM & bm) {
  for (int i=1; i!=BM::NVAL; ++i) {
     // the kernel to test
    //     bm.res[i] = std::atan2(bm.values0[i],bm.values2[i]);
     bm.res[i] = bm.values0[i]*bm.values2[i];
   }
  return (bm.res[k]>bm.dummies[k]) ? bm.res[0] : bm.res[BM::NVAL-k];
}

float stest(int k, BM & bm) {
  for (int i=1; i!=BM::NVAL; ++i) {
     // the kernel to test
    //     bm.res[i] = std::atan2(bm.values0[i],bm.values2[i]);
    bm.res[i] = bm.values0[i]/std::sqrt(bm.values1[i])/std::sqrt(bm.values2[i]);
   }
  return (bm.res[k]>bm.dummies[k]) ? bm.res[0] : bm.res[BM::NVAL-k];
}

float rtest(int k, BM & bm) {
  for (int i=1; i!=BM::NVAL; ++i) {
     // the kernel to test
    //     bm.res[i] = std::atan2(bm.values0[i],bm.values2[i]);
    bm.res[i] = bm.values0[i]/(std::sqrt(bm.values1[i]*bm.values2[i]));
   }
  return (bm.res[k]>bm.dummies[k]) ? bm.res[0] : bm.res[BM::NVAL-k];
}


int main() {
  BM bm;

  MARK(bm,atest);
  MARK(bm,dtest);
  MARK(bm,mtest);
  MARK(bm,stest);
  MARK(bm,rtest);

  return 0;
}
