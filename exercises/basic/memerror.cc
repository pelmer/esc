#include <cstdio>
#include <cstring>

void f(int argc, char **argv)
{
  printf("prog = '%s', n = %d\n", argv[0], argc);
}

int main(int, char **argv)
{
  int x;
  f(x, argv);
  return 0;
}
