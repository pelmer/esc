#include<iostream>
#include<cmath>

int  main() {
 float pi2=2.f*std::acos(-1.f);
 std::cout << pi2 << std::endl;
 std::cout << std::cos(1.f) << std::endl << std::endl;	

 float x=1.f;
 while (std::cos(x)==std::cos(x+pi2))
   x+=pi2;

 std::cout << x << std::endl;
 std::cout << std::cos(x) << " " << std::cos(x+pi2) << std::endl<< std::endl;


  x=1; float y=x;
 while (std::abs(std::cos(x)-std::cos(x+pi2))<1.e-4f) {
   y=x; x+=pi2; 
 }

 std::cout << y << " " << x << std::endl;
 std::cout << std::cos(x) << " " << std::cos(x+pi2) << std::endl<< std::endl;



  x=1;
  while (fabs(std::cos(1.f)-std::cos(x+pi2))<1.e-4f)
    x+=pi2;

 std::cout << x << std::endl;
 std::cout << std::cos(x) << " " << std::cos(x+pi2) << std::endl<< std::endl;


  return 0;
}
