#include<iostream>

void f() {
  int n=0; float w=1; float y=0;
  do {
    y = w++;
    ++n;
  } while (w>y);
  std::cout << n << " " << w << std::endl << std::endl;
}

int main() {
  f();
  return 0;
}

