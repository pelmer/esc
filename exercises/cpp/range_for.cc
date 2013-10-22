// compile with: c++ -std=c++11 range_for.cc

#include <vector>
#include <numeric>
#include <iostream>

int sum1(std::vector<int> const& v)
{
  int s = 0;
  for (auto i = 0u; i != v.size(); ++i) {
    s += v[i];
  }
  return s;
}

int sum2(std::vector<int> const& v)
{
  int s = 0;
  for (auto b = begin(v), e = end(v); b != e; ++b) {
    s += *b;
  }
  return s;
}

int sum3(std::vector<int> const& v)
{
  int s = 0;
  for (int i: v) {
    s += i;
  }
  return s;
}

int sum4(std::vector<int> const& v)
{
  return accumulate(begin(v), end(v), 0);
}

int main()
{
  std::vector<int> v {1, 2, 3, 4, 5};
  std::cout << sum1(v) << '\n';
  std::cout << sum2(v) << '\n';
  std::cout << sum3(v) << '\n';
  std::cout << sum4(v) << '\n';
}
