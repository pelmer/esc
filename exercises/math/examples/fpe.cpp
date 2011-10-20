#include<cmath>
#include<limits>
#include<cstring>
#include<cstdlib>
#include<cstdio>

#include <cerrno>
#include <csignal>

#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

#ifdef __SSE3__
#include <mmintrin.h>
#include <pmmintrin.h>
#endif

#include <features.h>
#include <fenv.h>



struct sigaction ignaction, termaction, newaction, oldaction;

std::string reportFPE() {
  int res = ::fetestexcept(FE_ALL_EXCEPT);
  std::string what;
  if (res & FE_DIVBYZERO ) what += "divbyzero ";
  if (res & FE_INVALID )  what += "invalid ";
  if (res & FE_OVERFLOW )  what += "overflow ";
  if (res & FE_UNDERFLOW ) what += "underflow ";
  if (res & FE_INEXACT ) what += "inexact ";
  // clear it 
  ::feclearexcept(FE_ALL_EXCEPT);
  return what;
}

void termSignalHandler(int sig) {
  std::cerr << "error " << sig << " " << reportFPE() << std::endl;
  exit(1000+sig);
}

namespace {
  enum FExp {div0=0, inv, over, under, inx, sqrn, log0, logn, roun, small};

  FExp what;

  char const * all = "diouxqlnrs";

}
namespace fpetest {

  extern float div(float x, float y);

  extern float prod(float x, float y);

  extern float sqr(float x);

  extern float ln(float x);

  extern float next(float x);


}


/*
double test(int i0, int n, double a)
{
double sum = 0.0;
int i;

for(i=i0; i<n; ++i)
  {
//    float x = 1.f/float(i);
    float x =  std::sqrt((float)i);
//    float x = logf((float)i);
//    sum += std::isnan(x) ? 0 : x;
    sum += (x!=x) ? 0 : x;
  }

return sum;
}
*/



int main(int n, char * opt[]) {

#ifdef __SSE3__
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif

  if (n<2) {
    std::cout << "please provide an option: d,i,o,u,x,q,l,n,r,s" << std::endl;
    return 0;
  }
      
  char o = opt[1][0];
  what = FExp(std::find(all,all+10,o) -all);

  std::cout << o << " " << what << std::endl;


  if (n>2) {

    (void) feenableexcept( FE_DIVBYZERO );
    (void) feenableexcept( FE_INVALID );
    (void) feenableexcept( FE_OVERFLOW );
    (void) feenableexcept( FE_UNDERFLOW );
    (void) feenableexcept( FE_INEXACT );
    
    sigset_t *def_set;
    def_set=&termaction.sa_mask;
    sigfillset(def_set);
    sigdelset(def_set,SIGFPE);
    termaction.sa_handler=termSignalHandler;
    termaction.sa_flags=0;
    sigaction(SIGFPE, &termaction,&oldaction);
  }

  float res=-9999.f;
  switch (what) {
  case div0:
    res = fpetest::div(3.f,-0.f);
    break;
  case inv:
    res = fpetest::div(-0.f,0.f);
    break;
  case roun:
    res = fpetest::div(1.f,10871LL);
    break;
  case under:
    res = fpetest::div(1.e-21f,1.e25f);
    // res = prod(1.e-28f,1.e-10f);
    break;
  case inx:
    res = fpetest::div(1.e-20f,1.e24f);
    // res = prod(1.e-28f,1.e-10f);
    break;
  case over:
    res = fpetest::div(1.e23f,1.e-23f);
    break;
  case sqrn:
    res = fpetest::sqr(-0.2f);
    break;
  case log0:
    res = fpetest::ln(0.0f);
    break;
  case logn:
    res = fpetest::ln(-0.2f);
    break;
  case small:
    res = fpetest::next(2.e-37f)-2.e-37f;
    break;
  }
  ::printf("%e\n", res);
  std::cout << "fpe " << reportFPE() << std::endl;


  /*
  ::printf("test(4, 6, 0) = %f\n", test(4,6,0));
  ::printf("test(0, 2, 0) = %f\n", test(0,2,0));
  ::printf("test(-2, 3, 0) = %f\n", test(-2,3,0));
  */
  return 0;
}
