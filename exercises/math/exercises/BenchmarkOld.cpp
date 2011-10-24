/*
 *
 *
 *
 *
 */
#include<cstdlib>
#include<iostream>
#include<cstdio>
#include <random>
#include <cmath>

const int NVAL=4096;
const int loops=100;

// globals: the compiler will properly allign and identify location (avoid aliasing)
// check against allocating on the heap in generate()
float values0[NVAL];
float values1[NVAL];
float values2[NVAL];
float res[NVAL];

float dummies[loops];

//  Default initialization used for random.
std::mt19937 eng;


void generate() {
  // random to avoid compiler optimization
  std::uniform_real_distribution<float> rgen(0.,1.e-6); 

  for (int i=0; i!=loops; ++i)
    dummies[i] = rgen(eng);
  
  values0[0] = -102.4 + rgen(eng);
  values1[0] = 102.4 + rgen(eng);
  values2[0] = rgen(eng);
  for (int i=1; i!=NVAL; ++i) {
    values0[i] = values0[i-1] + 0.05;
    values1[i] = values1[i-1] - 0.05;
    values2[i] = values2[i-1] + 0.05;
  }
}

volatile double dummy;
volatile double niterV;

template<typename F>
int bench(F fn) {
  int niter = loops;
  for (int i=0; i!=niter; ++i) {
    dummy += fn(i);
  }

  // fake: to avoid compier optimization
  if (dummy == 2.32132323232f) niter--;		\
  return niter;   
}


float vtest(int k) {
   for (int i=1; i!=NVAL; ++i) {
     // the kernel to test
     res[i] = std::atan2(values0[i],values2[i]);
   }
   return (res[k]>dummies[k]) ? res[0] : res[NVAL-k];
}

int main() {
  double e=0.;
  dummy=0;
  niterV=0;
  generate();

  // at least one second!
  while (e<1e6) { 
    double s=clock();
    niterV+=bench(vtest);
    e += clock()-s;
  }
  std::cout << "elapsed time is: " << e/niterV << std::endl;

  return 0;
}
