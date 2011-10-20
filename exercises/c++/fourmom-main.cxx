#include "fourmom.h"

// std includes
#include <iostream>

#define GeV 1.

int main(int /*argc*/, char **/*argv*/)
{
  FourMom mom;
  mom.set_px(20.*GeV);
  std::cout << "px: " << mom.px() << "\n";
  return 0;
}
