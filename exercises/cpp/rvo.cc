// compile with: c++ -std=c++11 rvo.cc

#include "String.h"

String urvo()
{
  return {}; // returns an unnamed default-constructed String
}

String nrvo()
{
  String result;
  return result; // returns a named default-constructed String
}

int main()
{
  LOG("URVO:");
  String s1 = urvo();
  LOG("NRVO:");
  String s2 = nrvo();
}
