#include<iostream>
#include<cstdio>

void f() {
  int n=0; float w=1; float y=0; float prev=0;
  do {
    prev=y;
    y = w++;
    ++n;
  } while (w>y);
  std::cout << n << ": " << prev << " " << y << " " << w << std::endl;
  printf("%a %a %a\n\n", prev,y,w);
}

int main() {
  f();
  return 0;
}

