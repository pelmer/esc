// compile with: c++ -std=c++11 -c auto.cc

void f()
{
  // auto a;               // error, no initializer
  auto i = 0;           // i has type int
  auto d = 0.;          // d has type double
  auto const e = 0.;    // e has type double const
  auto const& g = 0.;   // g has type double const&
  auto f = 0.f;         // f has type float
  auto c = "ciao";      // c has type char const*
  auto p = new auto(1); // p has type int*
  // auto int i;           // OK in C++98, error in C++11
}
