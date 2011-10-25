#include <cstddef>
#include <iostream>

class A
{
public:
  void m() { std::cout << "A::m()" << std::endl; }
  int ad;
};

class B : public A
{
public:
  void m() { std::cout << "B::m()" << std::endl; }
  int bd;
};

void f(A* p)
{
  p->ad = 5;
  p->m();
}

int
main(int, char **)
{
  A a;
  B b;

  std::cout << "size of a: " << sizeof(a)
	    << " offset of ad: " << offsetof(A, ad)
	    << std::endl;

  std::cout << "size of b: " << sizeof(b)
	    << " offset of ad: " << offsetof(B, ad)
	    << " offset of bd: " << offsetof(B, bd)
	    << std::endl;

  f(&a);
  f(&b);
  return 0;
}
