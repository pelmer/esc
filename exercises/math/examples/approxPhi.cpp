#include <emmintrin.h>
#include <cmath>

const float vl_epsilon_f  = 1.19209290E-07F; // guess what is this?

const double _2pi = (2.0 * 3.1415926535897932384626434);
const float _2pif = float(_2pi);
const float _pi4f = float(_2pi/8.);
float atanbuf_[257];
int atanbufdeg_[257];

namespace {
  // ====================================================================
  // arctan initialization
  // =====================================================================
  struct Initatan {
    Initatan() {
      unsigned int ind;
      for (ind = 0; ind <= 256; ind++) {
	double v = ind / 256.0;
	atanbuf_[ind] = ::atan(v);
	atanbufdeg_[ind] = ::lrint(4.*(360./_2pi)*::atan(v));
      }
    }
  };

  Initatan initan;
}


inline float
vl_fast_atan2_f (float y, float x) {
  float angle, r ;
  float const c3 = 0.1821F ;
  float const c1 = 0.9675F ;
  float abs_y    = std::abs(y) +  vl_epsilon_f;
  
  if (x >= 0) {
    r = (x - abs_y) / (x + abs_y) ;
    angle = _pi4f ;
  } else {
    r = (x + abs_y) / (abs_y - x) ;
    angle = 3.f * _pi4f ;
  }
  angle += (c3*r*r - c1) * r ;
  return (y < 0) ? - angle : angle ;
}
 

inline float phi(float y_, float x_) {
  unsigned int flags = 0;
  float t;
  union {
    float f;
    int i;
  } tp;
  tp.f = 32768.f; // 1/(256*2^-23)
  if (y_ < 0 ) { flags |= 4; y_ = -y_; }
  if (x_ < 0 ) { flags |= 2; x_ = -x_; }
  if (y_ > x_) {
    flags |= 1;
    _mm_store_ss( &t,_mm_rcp_ss( _mm_load_ss( &y_ ) ) );
    t *=x_;
    tp.f += t;
  }
  else {
    _mm_store_ss( &t,_mm_rcp_ss( _mm_load_ss( &x_ ) ) );
    t *=y_;
    tp.f += t;
  }
  int ind = (tp.i & 0x01FF);
  float th = atanbuf_[ind];
  if (flags & 1) { th = (_2pif / 4.f) - th; }
  if (flags & 2) { th = (_2pif / 2.f) - th; }
  if (flags & 4) { th = -th; }
  return th;
}

inline
int phideg(float y_, float x_) {
  unsigned int flags = 0;
  float t;
  union {
    float f;
    int i;
  } tp;
  tp.f = 32768.f;
  if (y_ < 0 ) { flags |= 4; y_ = -y_; }
  if (x_ < 0 ) { flags |= 2; x_ = -x_; }
  if (y_ > x_) {
    flags |= 1;
    _mm_store_ss( &t,_mm_rcp_ss( _mm_load_ss( &y_ ) ) );
    t *=x_;
    tp.f += t;
  }
  else {
    _mm_store_ss( &t,_mm_rcp_ss( _mm_load_ss( &x_ ) ) );
    t *=y_;
    tp.f += t;
  }
  int ind = (tp.i & 0x01FF);
  int th = atanbufdeg_[ind];
  if (flags & 1) { th = 360 - th; }
  if (flags & 2) { th = 720 - th; }
  if (flags & 4) { th = -th; }
  return th;
}


#include<iostream>
int main() {

  for (int i=0;i<=720;++i) {
    double th = double(i)*(_2pi/720.);
    double x = 1200.*cos(th);
    double y = 1200.*sin(th);
#ifndef NO_OUTPUT
    std::cout << std::atan2(y,x) << " " << phi(float(y),float(x))  << " " <<  vl_fast_atan2_f(float(y),float(x)) << std::endl;
    std::cout << std::atan2(y,x)*(360./_2pi) << " " << phi(float(y),float(x))*(360.f/_2pif) << std::endl;
    std::cout << int(std::atan2(y,x)*(4.*360./_2pi)) << " " << phideg(float(y),float(x)) << " " 
	      << int( vl_fast_atan2_f(float(y),float(x))*(4.f*360.f/_2pif)) << std::endl;
    std::cout << ::lrint(std::atan2(y,x)*(4.*360./_2pi)) << " " << phideg(float(y),float(x))  << " " 
	      << ::lrint( vl_fast_atan2_f(float(y),float(x))*(4.f*360.f/_2pif)) << std::endl;
#endif
    x *=0.001; y*=0.001;
    for (double r=0.001; r<10000.; r*=10) {
      x *=10; y*=10;
      if ( ::lrint(std::atan2(float(y),float(x))*(4.f*360.f/_2pif)) != phideg(float(y),float(x)))  std::cout << i << " differs"<< std::endl;  
    }
  }

  return 0;

}
