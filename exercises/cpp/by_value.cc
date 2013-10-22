// compile with: c++ -std=c++11 by_value.cc

#include "String.h"

class runtime_error
{
  String msg_;
public:
  runtime_error(String const& msg)
  : msg_(msg)
  {
  }
  // runtime_error(String msg)
  // : msg_(std::move(msg))
  // {
  // }
};

inline String get_string()
{
  String s("ESC 2013");
  if (true) { // to disable rvo
    return s;
  } else {
    return {};
  }
}

int main()
{
  String s("error");
  // pass an lvalue
  runtime_error e1(s);
  // pass a temporary
  runtime_error e2(get_string());
}
