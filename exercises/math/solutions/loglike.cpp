// g++ -O2 -std=gnu++0x -msse4 -ftree-vectorize -ftree-vectorizer-verbose=1 -pthread -fPIC -fopenmp  -lMinuit2
#include<random>
#include<cmath>
#include<iostream>
#include<limits>


#include "SSEArray.h"

float *  x;


std::ranlux48_base reng;
std::uniform_real_distribution<double> rgen(0.,1.);
std::normal_distribution<double> gauss1(0.1,1.);
std::normal_distribution<double> gauss2(-0.1,4.);


union fasi{
  int i;
  float x;
};



#include<cstdio>



void print(fasi const & f) {
 printf("%e %a %x\n",  f.x,  f.x,  f.i);
}

void look(float f) {
  std::cout << f << " ";
  fasi t; t.x = f; print(t);
}


void generate(size_t n) {
  for (size_t i= 0;  i!=n; ++i) {
    x[i] = (rgen(reng)<0.6) ? gauss1(reng) : gauss2(reng);
  }
}


float maxRes = std::numeric_limits<float>::max();
/*
inline float  __attribute__((__always_inline__))  onelog(float m, float s1, float s2, float r,  float x) {
  float delta1 = (x-m)*s1;
  float delta2 = (x-m)*s2;
  float res = (1.f-r)*std::exp(-delta1*delta1)*s1+r*std::exp(-delta2*delta2)*s2;
  return (res>1.e-15f) ? std::log(res) : std::log(1.e-15f);

  // float res = std::log((1.f-r)*std::exp(-delta1*delta1)*s1+r*std::exp(-delta2*delta2)*s2);
  // return (res>-maxRes) ? res : -maxRes; 
}
*/

inline float  __attribute__((__always_inline__))  onelog(float m, float s1, float s2, float r,  float x) {
  float delta1 = (x-m)*s1;
  float delta2 = (x-m)*s2;
  float res = (1.f-r)*mathSSE::exp(-delta1*delta1)*s1+r*mathSSE::exp(-delta2*delta2)*s2;
  return (res>1.e-15f) ? mathSSE::log(res) : mathSSE::log(1.e-15f);
}


// float
float loglikef(float m, float s1, float s2, float r, size_t n) {
  float res = 0.f;
  for (size_t i= 0;  i!=n; ++i) {
    res += onelog(m,s1,s2,r,x[i]);
  }
  return -res;
}

// double
float logliked(float m, float s1, float s2, float r, size_t n) {
  double res = 0;
  for (size_t i= 0;  i!=n; ++i) {
    res += onelog(m,s1,s2,r,x[i]);
  }
  return -res;
}

// kahan summation
float loglikeK(float m, float s1, float s2, float r, size_t n, float & c) {
  float res =  onelog(m,s1,s2,r,x[0]);
  c=0;
  for (size_t i= 1;  i!=n; ++i) {
    float y =  onelog(m,s1,s2,r,x[i]) - c;
    float t = res + y;
    c = (t - res) - y;
    res = t;
  }
  return -res;
}

namespace Which {
  enum V {f,d,k};
  V value;
}

inline float loglike(float m, float s1, float s2, float r, size_t n, float & c) {
  switch (Which::value) {
    case Which::f : 
      return loglikef(m, s1, s2, r, n);
    case Which::d : 
      return logliked(m, s1, s2, r, n);
    case Which::k :
      return loglikeK(m, s1, s2, r, n,c);

    }
}

#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/SimplexMinimizer.h"

using namespace ROOT::Minuit2;

struct LLFCN : public FCNBase {
  const size_t N;
  mutable float c;
  LLFCN(size_t n) : N(n), c(0) {
    maxRes = std::numeric_limits<float>::max()/float(N);
  }

  double operator()(const std::vector<double>& par) const {
    float m = par[0];
    float s1 = par[1];
    float s2 = par[2];
    float r = par[3];

    return loglike(m,s1,s2,r,N,c);
 
  }

  double Up() const {return 0.5;}


};


void minim(size_t N) {

  MnUserParameters upar;
  upar.Add("m",  0, 1.);
  upar.Add("s1", std::sqrt(2.f), 1.,0.01,100.);
  upar.Add("s2", std::sqrt(2.f)/6., 1.,0.01,100.);
  upar.Add("r", 0.5, .1,0.001,0.999);
  
  LLFCN fFCN(N);    
   
  MnMigrad migrad(fFCN, upar);
  std::cout<<"start migrad "<<std::endl;
  FunctionMinimum min = migrad();
  std::cout<<"minimum: "<<min<<std::endl;
  std::cout << "c " << fFCN.c << std::endl;
  if(!min.IsValid()) {
    //try with higher strategy
    std::cout<<"FM is invalid, try with strategy = 2."<<std::endl;
    MnMigrad migrad(fFCN, upar, 2);
    min = migrad();
  }
   std::cout<<"minimum: "<< min <<std::endl;
   std::cout << "c " << fFCN.c << std::endl;
}

int main(int n, char * opt[]) {

  if (n>1) {
    char o = opt[1][0];
    if (o=='f') Which::value = Which::f;
    else if (o=='k') Which::value = Which::k;
    else if (o=='d') Which::value = Which::d;
    
  }
  std::cout <<  Which::value << std::endl;

  const size_t N=1000000;

  x = new float[N];
  
  generate(N);
  float ff = std::sqrt(2.f);
  look(loglikef(0,ff,ff/4.f,0.4f,N));
  float c=0;
  look(loglikeK(0,ff,ff/4.f,0.4f,N,c));
  look(logliked(0,ff,ff/4.f,0.4f,N));


  minim(N);
   

 
 
  delete [] x;
  return 0;
}
