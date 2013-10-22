// compile with: c++ -std=c++11 lambda.cc

#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  copy(begin(v), end(v), std::ostream_iterator<T>(os, " "));
  return os;
}

int main()
{
  std::vector<int> v = {-2, -3, 4, 1};
  std::cout << v << '\n';

  sort(begin(v), end(v)); // default sort, v == {-3, -2, 1, 4}
  std::cout << v << '\n';

  struct abs_compare
  {
    bool operator()(int l, int r) const { return abs(l) < abs(r); }
  };

  sort(v.begin(), v.end(), abs_compare()); // C++98, v == {1, -2, -3, 4}
  std::cout << v << '\n';

  sort(begin(v), end(v), [](int l, int r) { return abs(l) < abs(r); }); // C++11
  std::cout << v << '\n';

  auto const it = find_if(begin(v), end(v), [] (int i) { return i < 0 && i % 2; });
  if (it != end(v)) {
    std::cout << *it << '\n';
  }
}
