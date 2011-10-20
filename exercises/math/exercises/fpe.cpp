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

#include <fenv.h>
#ifdef __linux__
#include <fpu_control.h>
#include <features.h>
#else
#ifdef __APPLE__
//TAKEN FROM
// http://public.kitware.com/Bug/file_download.php?file_id=3215&type=bug
static int
fegetexcept (void)
{
  static fenv_t fenv;

  return fegetenv (&fenv) ? -1 : (fenv.__control & FE_ALL_EXCEPT);
}

static int
feenableexcept (unsigned int excepts)
{
  static fenv_t fenv;
  unsigned int new_excepts = excepts & FE_ALL_EXCEPT,
               old_excepts;  // previous masks

  if ( fegetenv (&fenv) ) return -1;
  old_excepts = fenv.__control & FE_ALL_EXCEPT;

  // unmask
  fenv.__control &= ~new_excepts;
  fenv.__mxcsr   &= ~(new_excepts << 7);

  return ( fesetenv (&fenv) ? -1 : old_excepts );
}
/*
static int
fedisableexcept (unsigned int excepts)
{
  static fenv_t fenv;
  unsigned int new_excepts = excepts & FE_ALL_EXCEPT,
               old_excepts;  // all previous masks

  if ( fegetenv (&fenv) ) return -1;
  old_excepts = fenv.__control & FE_ALL_EXCEPT;

  // mask
  fenv.__control |= new_excepts;
  fenv.__mxcsr   |= new_excepts << 7;

  return ( fesetenv (&fenv) ? -1 : old_excepts );
}
*/
#endif
#endif



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

void init(bool flush, bool enable, bool trap) {

#ifdef __SSE3__
  if (flush) {
    std::cout << "flush to zero" << std::endl;
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
  }
#endif


  if (enable) {
    std::cout << "enabling FPE" << std::endl;	
    feenableexcept( FE_DIVBYZERO );
    feenableexcept( FE_INVALID );
    feenableexcept( FE_OVERFLOW );
    feenableexcept( FE_UNDERFLOW );
    feenableexcept( FE_INEXACT );
  }

  if (trap) {
    std::cout << "trapping FPE" << std::endl;	    
    sigset_t *def_set;
    def_set=&termaction.sa_mask;
    sigfillset(def_set);
    sigdelset(def_set,SIGFPE);
    termaction.sa_handler=termSignalHandler;
    termaction.sa_flags=0;
    sigaction(SIGFPE, &termaction,&oldaction);
  }
}


extern float execute(float x, float y) {
  return std::sqrt(x/y);
}

int main(int n, char * opt[]) {
     
  bool flush = false;
  bool trap = false;
  bool enable = false;
  while (n-- >1) {
    char o = opt[n][0];
    if (o=='f') flush=true;
    if (o=='t') trap=true;
    if (o=='e') enable=true;
  }
  init(flush,enable, trap);
  
  float res = execute(-1.f,3.f);

  ::printf("%e\n", res);
  std::cout << "fpe " << reportFPE() << std::endl;

  return 0;
}
