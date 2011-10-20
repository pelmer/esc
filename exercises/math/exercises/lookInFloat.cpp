#include<cmath>
#include<iostream>

void look(float x) {
  int e;
  float r = ::frexpf(x,&e);
  std::cout << x << " exp " << e << " res " << r << std::endl;
  
  union {
    float val;
    int bin;
  } f;
  
  f.val = x;
  printf("%e %a %x\n",  f.val,  f.val,  f.bin);
  int log_2 = ((f.bin >> 23) & 255) - 127;  //exponent
  f.bin &= 0x7FFFFF;                               //mantissa (aka significand)
  
  std::cout << "exp " << log_2 << " mant in binary "  << std::hex << f.bin 
	    << " mant as float " <<  std::dec << (f.bin|0x800000)*::pow(2.,-23)
	    << std::endl << std::endl;
}


float next(float x) {
  union {
    int i;
    float x;
  } tmp;

  tmp.x=x;
  tmp.i++;
  return tmp.x;

}

float prev(float x) {
  union {
    int i;
    float x;
  } tmp;

  tmp.x=x;
  tmp.i--;
  return tmp.x;

}


main() {

  float x = 1.f/3.f;
  look(x);
  look(prev(x));
  look(next(x));


  return 0;
}

