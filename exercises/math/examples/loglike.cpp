// g++ -O2 -std=gnu++0x -msse4 -ftree-vectorize -ftree-vectorizer-verbose=1 -pthread -fPIC -fopenmp  -lMinuit2
#include<random>
#include<cmath>
#include<iostream>


float *  x;


std::ranlux48_base reng;
std::uniform_real_distribution<double> rgen(0.,1.);
std::normal_distribution<double> gauss1(0.1,1.);
std::normal_distribution<double> gauss10(-0.1,4.);


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
    x[i] = (rgen(reng)<0.6) ? gauss1(reng) : gauss10(reng);
  }
}

inline float onelog(float m, float s1, float s2, float r,  float x) {
  float delta1 = (x-m)/s1;
  float delta2 = (x-m)/s2;
    return std::log((1.-r)*std::exp(-0.5*delta1*delta1)/s1+r*std::exp(-0.5*delta2*delta2)/s2);
  }

float loglike0(float m, float s1, float s2, float r, size_t n) {
  float res = 0;
  for (size_t i= 0;  i!=n; ++i) {
    res += onelog(m,s1,s2,r,x[i]);
  }
  return -res;
}

float logliked(float m, float s1, float s2, float r, size_t n) {
  double res = 0;
  for (size_t i= 0;  i!=n; ++i) {
    res += onelog(m,s1,s2,r,x[i]);
  }
  return -res;
}


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


#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/SimplexMinimizer.h"

using namespace ROOT::Minuit2;

struct LLFCN : public FCNBase {
  bool useK;
  const size_t N;

  LLFCN(bool k,size_t n) : useK(k), N(n), opar(3,0.) {
    old0.x=0;
    oldK.x=0;

  }

  double operator()(const std::vector<double>& par) const {
    float m = par[0];
    float s1 = par[1];
    float s2 = par[2];
    float r = par[3];

    float res0 = loglike0(m,s1,s2,r,N);
    float c=0;
    float resK = loglikeK(m,s1,s2,r,N,c);
    old0.x-=res0;
    oldK.x-=resK;
    if ((old0.i>>31) != (oldK.i>>31) ) {
      //      printf("%f %f %f %f\n",m,s1,s2,r);
      printf("%f %f %f %f\n",par[0],par[1],par[2],par[3]);
      printf("%f %f %f %f\n",opar[0],opar[1],opar[2],opar[3]);
      look(old0.x); look(oldK.x); 
      look(res0); look(resK);  look(logliked(m,s1,s2,r,N));
    }
    old0.x=res0;
    oldK.x=resK;
    opar=par;
 
    return useK ? resK : res0;

  }

  mutable std::vector<double> opar;
  mutable fasi old0;
  mutable fasi oldK;


  double Up() const {return 0.5;}


};


void minim(bool k, size_t N) {

  MnUserParameters upar;
  upar.Add("m",  0, 1.);
  upar.Add("s1", 1., 1.,0.,100.);
  upar.Add("s2", 6., 1.,0.,100.);
  upar.Add("r", 0.5, .1,0.,1.);
  
  LLFCN fFCN(k,N);    
   
  MnMigrad migrad(fFCN, upar);
  std::cout<<"start migrad "<<std::endl;
  FunctionMinimum min = migrad();
  if(!min.IsValid()) {
    //try with higher strategy
    std::cout<<"FM is invalid, try with strategy = 2."<<std::endl;
    MnMigrad migrad(fFCN, upar, 2);
    min = migrad();
  }
   std::cout<<"minimum: "<<min<<std::endl;
   
}

int main() {

  const size_t N=1000000;

  x = new float[N];
  
  generate(N);
  
  look(loglike0(0,1.,4.,0.4,N));
  float c=0;
  look(loglikeK(0,1.,4.,0.4,N,c));
  look(logliked(0,1.,4.,0.4,N));

  /*
  float tmin=1.e30;
  float tmax=0;
  fasi old0; old0.x=0;
  fasi oldK; oldK.x=0;
  for (float ds1=-0.5; ds1<1; ds1+=0.3) 
    for (float ds2=-3.; ds2<3.; ds2+=1.) 
      for (float dr=-0.3;  dr<0.3; dr+=0.3) 
	for (float m=-2; m<2; m+=0.5) 
	  {
	    float res0 = loglike0(m,1.+ds1,4.+ds2,0.4+dr,N);
	    float c=0;
	    float resK = loglikeK(m,1.+ds1,4.+ds2,0.4+dr,N,c);
	    old0.x-=res0;
	    oldK.x-=resK;
	    if ((old0.i>>31) != (oldK.i>>31) ) {
	      printf("%f %f %f %f\n",m,ds1,ds2,dr);
	      look(old0.x); look(oldK.x);
	      look(res0); look(resK);
	    }
	    old0.x=res0;
	    oldK.x=resK;
	    if (resK<tmin) tmin=resK;
	    if (resK>tmax) tmax=resK;
	  }
	  std::cout << tmin << " " << tmax << std::endl;
  */


  minim(true,N);
  minim(false,N);
   

  

 
  delete [] x;
  return 0;
}
