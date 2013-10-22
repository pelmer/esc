// compile with: c++ -std=c++11 initializer_list.cc

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <iterator>

int main()
{
  std::vector<int> const v = {1, 2, 3, 4, 5};
  std::cout << v.size() << '\n';
  copy(begin(v), end(v), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  std::map<int, std::string> const ids = {
    {23, "Andrea"},
    {49, "Camilla"},
    {96, "Ugo"},
    {72, "Elsa"}
  };
  std::cout << ids.size() << '\n';
}
