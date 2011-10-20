// Compile: c++ -g -W -Wall -Werror -ansi -pedantic -o vvvi-reserve vvvi-reserve.cc -ldl -lpthread
#include <vector>

int main (int, char **)
{
  typedef std::vector<int> VI;
  typedef std::vector<VI> VVI;
  std::vector<VVI> vvvi;
  vvvi.reserve(10);
  for (int i = 0, j, k; i < 10; ++i)
  {
    vvvi.push_back(VVI());
    vvvi.back().reserve(10);
    for (j = 0; j < 10; ++j)
    {
      vvvi.back().push_back(VI());
      vvvi.back().back().reserve(10);
      for (k = 0; k < 10; ++k)
        vvvi.back().back().push_back(k);
    }
  }
  return vvvi.size();
}
