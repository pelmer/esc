// Compile: c++ -g -W -Wall -Werror -ansi -pedantic -o array-smash array-smash.cc -ldl -lm -lpthread
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>

struct mytest
{
  std::vector<double> d;
};

void init(std::vector<mytest *> &t)
{
  for (int n = 0; n < 10; ++n)
    t.push_back(new mytest);
}

void fill(std::vector<mytest *> &t)
{
  for (int n = 0; n <= 10; ++n)
  {
    t[n]->d.resize(63, 0);
    for (int i = 0; i <= 63; ++i)
      t[n]->d[i] = sin(M_PI / 10. * i);
  }
}

double sum(std::vector<mytest *> &t)
{
  double sum = 0;
  for (int n = 0; n <= 10; ++n)
    for (int i = 0; i <= 63; ++i)
      sum += t[n]->d[i];
  return sum;
}

int main(int, char **)
{
  std::vector<mytest *> t;
  init (t);
  fill (t);
  std::cout << t.size() << ", " << sum(t) << std::endl;
  return 0;
}
