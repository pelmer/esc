#include <iostream>
#include <stdint.h>

class A
{
public:
  virtual void m1() { std::cout << "A::m1()" << std::endl; }
  virtual void m2() { std::cout << "A::m1()" << std::endl; }
  int ad;
};

class B: public A
{
public:
  virtual void m1() { std::cout << "B::m1()" << std::endl; }
  int bd;
};

// prints the vtable of a given object
void
print_vtable(A *pa)
{
  // p sees pa as an array of dwords
  uintptr_t *p = reinterpret_cast<uintptr_t*>(pa);
  // vt sees vtable as an array of pointers
  void **vt = reinterpret_cast<void**>(p[0]);
  std::cout << std::hex << "vtable address = " << vt << std::endl;
  for (int i = 0; i < 2; ++i) {
    std::cout << "dword " << i << ": " << vt[i] << std::endl;
  }

  // call virtual functions in an EXTREMELY NON-PORTABLE way
  void (*m1fct_ptr)(A*); // fct pointer
  m1fct_ptr = reinterpret_cast<void(*)(A*)>(vt[0]);
  m1fct_ptr(pa); // call method m1

  void (*m2fct_ptr)(A*); // fct pointer
  m2fct_ptr = reinterpret_cast<void(*)(A*)>(vt[1]);
  m2fct_ptr(pa); // call method m1

}

int main(int, char **)
{
  A a;
  B b1;
  B b2;

  std::cout << "size of a: " << sizeof(a)
	    << " offset of ad: " << offsetof(A, ad)
	    << std::endl;

  std::cout << "size of b: " << sizeof(b1)
	    << " offset of ad: " << offsetof(B, ad)
	    << " offset of bd: " << offsetof(B, bd)
	    << std::endl;


  std::cout << "a:  " << std::endl; print_vtable(&a);
  std::cout << "b1: " << std::endl; print_vtable(&b1);
  std::cout << "b2: " << std::endl; print_vtable(&b2);
  return 0;
}
