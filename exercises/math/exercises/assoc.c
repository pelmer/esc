#include<math.h>

double sqrt0(double x, double y) {
   return __builtin_sqrt(x)*__builtin_sqrt(y);
}

double sqrt1(double a, double x, double y) {
   return a*__builtin_sqrt(x)*__builtin_sqrt(y);
}

double sqrt2(double a, double x, double y) {
   return a*sqrt(x)*sqrt(y);
}

double exp0(double x, double y) {
   return exp(x)*exp(y);
}

double exp1(double a, double x, double y) {
   return a*exp(x)*exp(y);
}

double exp02(double a, double x, double y) {
  return a*(exp(x)*exp(y));
}

double exp03(double a, double x, double y) {
  double tmp1 = exp(x);
  double tmp2 = exp(y);
  return a*(tmp1*tmp2);
}
