// Compile: c++ -g -W -Wall -Werror -ansi -pedantic -o vvvi-build-and-copy vvvi-build-and-copy.cc -ldl -lpthread
#include <vector>
#include <string>
#include <dlfcn.h>

int main (int, char **)
{
  union { void *ptr; void (*dump)(const char *tofile); } u;
  u.ptr = dlsym(0, "igprof_dump_now");

  typedef std::vector<int> VI;
  typedef std::vector<VI> VVI;
  std::vector<VVI> vvvi, vvvi2;
  for (int i = 0, j, k; i < 10; ++i)
    for (vvvi.push_back(VVI()), j = 0; j < 10; ++j)
      for (vvvi.back().push_back(VI()), k = 0; k < 10; ++k)
        vvvi.back().back().push_back(k);

  if (u.dump) u.dump("|gzip -9c>vvvi-build.gz");

  vvvi2 = vvvi;
  if (u.dump) u.dump("|gzip -9c>vvvi-copy.gz");
  return vvvi2.size();
}
