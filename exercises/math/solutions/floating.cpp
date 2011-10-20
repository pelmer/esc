#include<cmath>
#include<cstdio>
#include<iostream>



void f() {
  int n=0; float w=1; float y=0;
  do {
    y = w++;
    ++n;
  } while (w>y);
  std::cout << n << " " << w << std::endl;
}
void i() {
  int w=0; int y=0;
  do { y = w++; } while (w>y);
  std::cout << w << std::endl;
}
void f2() {
  float x = 1.f/3.f;
  float y = std::sqrt(1.f/x/x);
  float z = 1.f/std::sqrt(x*x);
  std::cout << std::boolalpha;
  std::cout << z << " " << y << std::endl;
  std::cout << (z==x) << " " << (z<x) << " " << (z>x) << std::endl;
}
int main() {
  f();
  f2();
  i();
  return 0;
}
