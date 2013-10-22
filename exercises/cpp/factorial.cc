// compile with: c++ -std=c++11 factorial.cc

#include <iostream>

template<int N>
struct Factorial
{
  static const int value = N * Factorial<N-1>::value;
};

template<>
struct Factorial<1>
{
  static const int value = 1;
};

constexpr int factorial(int N)
{
  return N < 2 ? 1 : N * factorial(N - 1);
}

constexpr int factorial_aux(int F, int N)
{
  return N < 2 ? F : factorial_aux(F * N, N - 1); // tail recursion
}

constexpr int factorial1(int N)
{
  return factorial_aux(1, N);
}

int main()
{
  std::cout << Factorial<10>::value << '\n';
  std::cout << factorial(10) << '\n';
  constexpr size_t f = factorial(10);
  std::cout << f << '\n';
  static_assert(Factorial<10>::value == factorial(10), "Factorial<10>::value == factorial(10)");
}
