#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdint.h>
#include <cmath>

#define RDTSC(v)							\
  do { unsigned lo, hi;							\
    __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));			\
    (v) = ((uint64_t) lo) | ((uint64_t) hi << 32);			\
  } while (0)

double test1(int niter)
{
  double sum = 0.;
  double step = M_PI / niter;
  double val = 0.;
  uint64_t total_time = 0;

  for (int i = 0; i < niter; ++i, val += step)
  {
    uint64_t start, end;

    RDTSC(start);
    sum += sin(val);
    RDTSC(end);

    uint64_t dtime = end - start;
    std::cout << "#" << i << " " << dtime << std::endl;
    total_time += dtime;
  }

  std::cout << "TOTAL " << total_time << std::endl;
  return sum;
}

double test2(int niter)
{
  double sum = 0.;
  double step = M_PI / niter;
  double val = 0.;
  uint64_t total_time = 0;
  uint64_t start, end;

  RDTSC(start);
  for (int i = 0; i < niter; ++i, val += step)
    sum += sin(val);
  RDTSC(end);

  total_time = end - start;
  std::cout << "TOTAL " << total_time << std::endl;
  return sum;
}

int main(int argc, char **argv)
{
  int niter = 10;
  if (argc > 1)
    niter = atoi(argv[1]);

  double t1 = test1(niter);
  std::cout << "test1(): " << std::setprecision(3) << t1 << std::endl;

  double t2 = test2(niter);
  std::cout << "test2(): " << std::setprecision(3) << t2 << std::endl;

  return 0;
}
