#include<cmath>

namespace fpetest {
  float div(float x, float y) {
    return x/y;	
  } 
  
  float prod(float x, float y) {
    return x*y;
  }
  
  float sqr(float x) {
    return std::sqrt(x);
  }
  
  float ln(float x) {
    return std::log(x);
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


}
