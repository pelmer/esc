// compile with: c++ -std=c++11 main_string.cc

#include "String.h"
#include <utility>

inline String get_string()
{
  String s("ESC 2013");
  if (true) { // to disable rvo
    return s;
  } else {
    return {};
  }
}

void f(String const& s)
{
  LOG("f(String const&)");
  LOG(s);
}

void f(String&& s)
{
  LOG("f(String&&)");
  LOG(s);
}

int main()
{
  String s1("Hello");
  String s2(s1);
  String s3(get_string());
  String s4(std::move(s3));

  f(s1);
  f(get_string());
}

