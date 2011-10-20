#include<cmath>
float conv(float x) {
  x+=3.14;
  return pow(x,2);
}

float noconv(float x) {
  x+=3.14f;
  return std::pow(x,2.f);
}
