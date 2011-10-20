// Compile: c++ -g -W -Wall -Werror -ansi -pedantic -o vvs-build-and-copy vvs-build-and-copy.cc -ldl -lpthread
#include <vector>
#include <string>
#include <dlfcn.h>

int main (int, char **)
{
  union { void *ptr; void (*dump)(const char *tofile); } u;
  u.ptr = dlsym(0, "igprof_dump_now");

  std::vector< std::vector<std::string> > vvs, vvs2;
  for (int i = 0; i < 100; ++i) {
    vvs.push_back(std::vector<std::string>());
    for (int j = 0; j < 10; ++j)
      vvs.back().push_back("junk");
  }

  if (u.dump) u.dump("|gzip -9c>vvs-build.gz");

  vvs2 = vvs;
  if (u.dump) u.dump("|gzip -9c>vvs-copy.gz");

  return vvs2.size();
}
