#include<iostream>
#include<limits>
void i() {
  int a = std::numeric_limits<int>::max();
  int b = a+1;
  std::cout << a << " " << b << " " << (b>a)  << std::endl; 

  int w=0; int y=0;
  bool wrap=false;
  do { y = w++; 
       if (y==0||y==-1||y==std::numeric_limits<int>::max()) { wrap=true; std::cout << w << " " << w+1 << std::endl;}
       if (wrap) { wrap=false; std::cout << y << " " << w << std::endl;}
     } while (w>y);
  std::cout << w << std::endl << std::endl;
}

int main() {
  i();
  return 0;
}
