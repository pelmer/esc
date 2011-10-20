// local includes
#include "exception-lib1.h"
#include "exception-lib2.h"

// std includes
#include <stdio.h>

// no error handling, you should add it...
double g(int x, int y, int z)
{ 
  return f1(x) + f2(y) + f3(z); 
}


void
run_g_fct(int x, int y, int z)
{
  double result = g(x,y,z);
  printf("result: %8.3f\n", result);
  return;
}
