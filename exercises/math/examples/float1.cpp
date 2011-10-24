#include<cmath>
#include<cstdio>
#include<iostream>


void i() {
  int w=0; int y=0;
  do { y = w++; } while (w>y);
  std::cout << w << std::endl << std::endl;
}


void f() {
  int n=0; float w=1; float y=0;
  do {
    y = w++;
    ++n;
  } while (w>y);
  std::cout << n << " " << w << std::endl << std::endl;
}


inline void f2(float x) {
  float y = std::sqrt(1.f/x/x);
  float z = 1.f/std::sqrt(x*x);
  std::cout << std::boolalpha;
  std::cout << x << " " << z << " " << y << std::endl;
  std::cout << (z==y) << " " << (z<y) << " " << (z>y) << " " << (z-y) << std::endl;
  printf("%e %e %e\n", x,y,z);
  printf("%a %a %a\n\n", x,y,z);
}

inline void f3(float x1, float x2) {
  float y = x1*x1 - x2*x2;
  float z = (x1+x2)*(x1-x2);
  std::cout << std::boolalpha;
  std::cout << x1 << " " << x2 << std::endl;
  std::cout << z << " " << y << std::endl;
  std::cout << (z==y) << " " << (z<y) << " " << (z>y) << std::endl;
  printf("%e %e %e %e\n", x1,x2,y,z);
  printf("%a %a %a %a\n\n", x1,x2,y,z);
}



void fcos(float pi2) {
  printf("2pi = %g, %a\n",pi2,pi2);
  printf("cos(2pi) = %g, %a\n",std::cos(pi2),std::cos(pi2));
  float x=1.f;
  while (std::cos(x)==std::cos(x+pi2))
    x+=pi2;
  std::cout << x << " " << std::cos(x) << std::endl;
  std::cout << x+pi2 << " " << std::cos(x+pi2) <<  std::endl;
  printf("%a %a\n",std::cos(x), std::cos(x+pi2));
  printf("%a %a\n",std::cos(nextafterf(x+pi2,0.f)),std::cos(nextafterf(x+pi2,100.f)));
  printf("%a %a\n\n",std::cos(x+nextafterf(pi2,0.f)),std::cos(x+nextafterf(pi2,100.f)));
  
  x=1;
  while (fabs(std::cos(x)-std::cos(x+pi2))<1.e-5f)
    x+=pi2;
  std::cout << x << " " << std::cos(x) << std::endl;
  std::cout << x+pi2 << " " << std::cos(x+pi2) << std::endl<< std::endl;
  
  
  x=1; float y=x;
  while (std::abs(std::cos(x)-std::cos(x+pi2))<1.e-4f) {
    y=x; x+=pi2; }
  std::cout << x << " " << std::cos(x) << std::endl;
  std::cout << y << " " << std::cos(y) << std::endl;
  std::cout << x+pi2 << " " << std::cos(x+pi2) << std::endl<< std::endl;
  
  x=1;
  while (fabs(std::cos(1.f)-std::cos(x+pi2))<1.e-4f)
    x+=pi2;
  std::cout << x << " " << std::cos(x) << std::endl;
  std::cout << x+pi2 << " " << std::cos(x+pi2) << std::endl<< std::endl;
  
  
}

int main() {
  f();
  f2(1.f/3.f);
  f2(1.f/3.f+3.f/7.f);
  f2(1.f/(1024.f*3.f)+3.f/(1024.f*7.f));
  f2(1.f/(1000.f*3.f)+3.f/(1000.f*7.f));
  
  f3(1.f/3.f,1.f/5.f);
  f3(300.f+1.f/3.f,4.f+1.f/3.f);
  

  float pi2=2.*3.141592653589793238;
  fcos(pi2);
  pi2=2.f*std::acos(-1.f);
  fcos(pi2);
  fcos(nextafterf(pi2,1.f));
  // i();
  return 0;
}
