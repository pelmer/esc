#include "exception-lib1.h"
#include "exception-lib2.h"

#include <stdio.h>

int main(int /*argc*/, char **/*argv*/)
{
  // alternatively, take x,y,z from command arguments...
  run_g_fct(1,2,3);
  return 0;
}
