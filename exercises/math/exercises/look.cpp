#include<numeric>
#include<cstdio>
#include<limits>
#include<cmath>
#include<iostream>


  void look(float x) {
  printf("%a\n",x);
  int e; float r = ::frexpf(x,&e);
  std::cout << x << " exp " << e << " res " << r << std::endl;
  	
  union {
    float val;
    int bin;
  } f;

  f.val = x;
  printf("%e %a %x\n", f.val, f.val,  f.bin);
  int log_2 = ((f.bin >> 23) & 255) - 127;  //exponent
  f.bin &= 0x7FFFFF;                               //mantissa (aka significand)

  std::cout << "exp " << log_2 << " mant in binary "  << std::hex << f.bin 
	    << " mant as float " <<  std::dec << (f.bin|0x800000)*::pow(2.,-23)
                   << std::endl << std::endl;
 }

int dist(float x,float y) {
  union {
    float val;
    int bin;
  } f;

  f.val = x;
  int a= f.bin;
  f.val = y;
  int b= f.bin;

  std::cout << "dist " << x << " " << y << ": " << a-b << std::endl; 
  return a-b;

}

int main(int npar, char * []) {

  constexpr auto maxf = std::numeric_limits<float>::max();
  constexpr auto minf = std::numeric_limits<float>::min();
  
  std::cout << int(pow(2.,23)) << " " << (1<<23) << std::endl;

  look(1.f);
  look(-2.f);
  look(0.5f);
  look(maxf);
  look(minf);
  look(0);
  look(-0);
  look(maxf+3.);
  look(-maxf-3.);
  look(std::sqrt(-1.f));
  look(1.f/3.f);
  look(std::acos(-1.f));
  look(2.f*std::asin(1.f));
  look(1.e6);
  look(2.e6);

  dist(1.f,2.f);
  dist(2.f,3.f);
  dist(10.f,20.f);
  dist(10000.f,10002.f);
  dist(50000.f,50002.f);
  dist(100000.f,100002.f);
  dist(500000.f,500002.f);
  dist(1.e6f,1.e6f+2.f);
  dist(1.e6f,2.e6f);
  float zz = 1.23456789e9;
  dist(zz,2*zz);

  
  return 0;
}
