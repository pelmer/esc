#include<cstdlib>
#include<iostream>
#include<cstdio>
#include <random>
#include <cmath>
#include <typeinfo>

volatile double dummy;
volatile double niterV;
  

template<typename T, int N>
struct Benchmark {


  static constexpr int NVAL=N;
  static constexpr int loops=100;


  T values0[NVAL];
  T values1[NVAL];
  T values2[NVAL];
  T res[NVAL];
  
  T dummies[loops];

  //  Default initialization used for random.
  std::mt19937 eng;
  
  
  void generate() {
    // random to avoid compiler optimization
    std::uniform_real_distribution<T> rgen(0.,1.e-6); 
    T step = 0.05;
    T lim = step*T(NVAL);
    for (int i=0; i!=loops; ++i)
      dummies[i] = rgen(eng);
    
    values0[0] = -lim + rgen(eng);
    values1[0] =  lim + rgen(eng);
    values2[0] = rgen(eng);
    for (int i=1; i!=NVAL; ++i) {
      values0[i] = values0[i-1] + step;
      values1[i] = values1[i-1] - step;
      values2[i] = values2[i-1] + step;
    }
  }
  
  
  template<typename F>
  int bench(F fn) {
    int niter = loops;
    for (int i=0; i!=niter; ++i) {
      dummy += fn(i,*this);
    }

    // fake: to avoid compier optimization
    if (dummy == 2.32132323232f) niter--;	\
    return niter;   
  }


  template<typename F>
  double mark(F fn) {
    double e=0.;
    dummy=0;
    niterV=0;
    generate();
    
    // at least one second!
    while (e<1e6) { 
      double s=clock();
      niterV+=bench(fn);
      e += clock()-s;
    }
    return e/niterV;
  }
    
};

#define MARK(_bm,_f) \
  double res##_f = _bm.mark(_f); \
  std::cout << #_f << " elapsed time is: " <<  res##_f << std::endl
