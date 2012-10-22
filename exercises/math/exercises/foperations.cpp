#include<cmath>
#include<cstdio>
#include<iostream>

inline void f2(float x) {
  float y = std::sqrt(1.f/x/x);
  float z = 1.f/std::sqrt(x*x);
  std::cout << "\nsqrt(1/x/x) vs 1/sqrt(x*x)" << std::endl;	
  std::cout << std::boolalpha;
  std::cout << x << " " << z << " " << y << std::endl;
  std::cout << "=<>- " << (z==y) << " " << (z<y) << " " << (z>y) << " " << (z-y) << std::endl;
  printf("%e %e %e\n", x,y,z);
  printf("%a %a %a\n\n", x,y,z);
}

inline void f3(float x1, float x2) {
  float y = x1*x1 - x2*x2;
  float z = (x1+x2)*(x1-x2);
  std::cout << "\nx1*x1 - x2*x2 vs (x1+x2)*(x1-x2)" << std::endl;
  std::cout << std::boolalpha;
  std::cout << x1 << " " << x2 << std::endl;
  std::cout << z << " " << y << std::endl;
  std::cout << "=<>- " << (z==y) << " " << (z<y) << " " << (z>y) << " " << (z-y) << std::endl;
  printf("%e %e %e %e\n", x1,x2,y,z);
  printf("%a %a %a %a\n\n", x1,x2,y,z);
}



int main() {
  f2(1.f/3.f);
  f2(1.f/3.f+3.f/7.f);
  f2(1.f/(1024.f*3.f)+3.f/(1024.f*7.f));
  f2(1.f/(1000.f*3.f)+3.f/(1000.f*7.f));
  
  f3(1.f/3.f,1.f/5.f);
  f3(300.f+1.f/3.f,4.f+1.f/3.f);

  return 0;
}
