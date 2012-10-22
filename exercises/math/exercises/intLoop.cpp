#include<iostream>
#include<limits>
void i() {
  int w=0; int y=0;
  do { y = w++; } while (w>y);
  std::cout << y << " " << w << std::endl << std::endl;
}

int main() {
  i();
  return 0;
}
