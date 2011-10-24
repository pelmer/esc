#include <iostream>

int main(int n, char ** v) {

  float x = 1.e10f;
  float	y = x+float(n);
  float z = y-x;
  
  if (float(n)==z) std::cout << "compiled with fast-math!" << std::endl;
  else std::cout << "compiled with ieee754-math!" << std::endl;

  return 0;
}
