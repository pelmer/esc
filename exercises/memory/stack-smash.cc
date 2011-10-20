// Compile: c++ -g -W -Wall -Werror -ansi -pedantic -o stack-smash stack-smash.cc -ldl -lpthread
#include <cstdio>
#include <cstring>

int g(char *buf)
{
  return sprintf(buf, "This message will not quite fit into the buf.");
}

void f()
{
  char buf[10];
  int len = g(buf);
  printf("buf = '%s', len = %d", buf, len);
}

int main(int, char **)
{
  f();
  return 0;
}
