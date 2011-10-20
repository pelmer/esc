#include<cmath>
#include<cstdio>
#include<iostream>

union fasi {
    float val;
    int bin;
  };
  

bool noeq(fasi a, fasi b) {
  return std::abs(a.bin-b.bin)>1;
}

void look(float x) {
  fasi f; f.val=x;
  printf("%e %a %x\n",  f.val,  f.val,  f.bin);
}

void look(fasi f) {
  printf("%e %a %x\n",  f.val,  f.val,  f.bin);
}

void  a() {

  float x=1.f/3.f;
  look(x);
  fasi z; z.val=1;
  fasi y; y.val=.1;
  while(noeq(z,y)) {
    y.val = std::sqrt(z.val*z.val-x*x);
    z.bin++;
  }
  look(y);
  look(z);
  std::cout << std::endl;
}

void  a2() {

  float x=1.e-10f/3.f;
  look(x);
  fasi z; z.val=1;
  float y=0;
  while(z.val*z.val>y) {
    y = z.val*z.val-x*x;
    z.bin++;
  }
  look(y);
  look(z.val);
  look(z.val*z.val);
  std::cout << std::endl;
}


void  b0() {
  float x=1.e-10f/3.f;
  look(x);
  fasi z; z.val=1;
  float y=0;
  while(z.val>y) {
    y = std::sqrt((z.val-x)*(z.val+x));
    z.bin++;
  }
  look(y);
  look(z.val);
  std::cout << std::endl;
}

void  b() {
  float x=1.f/3.f;
  look(x);
  fasi z; z.val=1;
  fasi y; y.val=.1;
  while(noeq(z,y)) {
    y.val = std::sqrt((z.val-x)*(z.val+x));
    z.bin++;
  }
  look(y);
  look(z);
  look(z.val+x);
  look(z.val-x);
  std::cout << std::endl;
}

void  b2() {
  float x=1.e-15f/3.f;
  look(x);
  fasi z; z.val=1;
  float y=0;
  while(z.val*z.val>y) {
    y = (z.val-x)*(z.val+x);
    z.bin++;
  }
  look(y);
  look(z.val);
  look(z.val*z.val);
  look(z.val+x);
  look(z.val-x);
  fasi z1  = z; z1.bin--;
  look(z.val-z1.val);
  std::cout << std::endl;

}


int main() {
  a();
  a2();
  b();
  b2();
}
