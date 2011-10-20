#include<cmath>
#include<iostream>
#include<limits>


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

  std::cout << std::boolalpha;
  std::cout << "Minimum value for float: " << std::numeric_limits<float>::min() << std::endl;
  std::cout << "Minimum value for float: " << std::numeric_limits<float>::denorm_min() << std::endl;
  std::cout << "Maximum value for float: " << std::numeric_limits<float>::max() << std::endl;
  std::cout << "float is signed: " << std::numeric_limits<float>::is_signed << std::endl;
  std::cout << "Non-sign bits in float: " << std::numeric_limits<float>::digits << std::endl;
  std::cout << "float has infinity: " << std::numeric_limits<float>::has_infinity << std::endl;
  std::cout << std::endl;
  
  look(std::numeric_limits<float>::quiet_NaN());
  look(std::numeric_limits<float>::infinity());
  look(std::numeric_limits<float>::epsilon());
  look(std::numeric_limits<float>::round_error());

  std::cout << "is nan? " << !(std::numeric_limits<float>::quiet_NaN()==std::numeric_limits<float>::quiet_NaN()) << "\n" << std::endl;
  
  
  float x = 1.f/3.f;
  look(x);
  look(prev(x));
  look(next(x));
  look(next(x)-x);
  float y = std::exp(std::log(std::sqrt(x*x)));
  look(y);
  

  union {
    float val;
    int bin;
  } one, two, four;

  one.val=1.f;
  two.val=2.f;
  four.val=4.f;
  std::cout << two.bin-one.bin << " " << log2(two.bin-one.bin) << std::endl;
  std::cout << four.bin-two.bin << " " << log2(four.bin-two.bin) << std::endl;
  
  return 0;

}

