// compile with: c++ -std=c++11 noexcept.cc

struct S
{
  void f() noexcept
  {
    throw 1;
  }
};

struct R
{
  void f()
  {
    throw 'a';
  }
};

struct E
{
  template<typename T>
  void g(T t) noexcept(noexcept(t.f()))
  {
    return t.f();
  }
};

int main()
{
  try {
    E e;
    S s;
    e.g(s);
    // R r;
    // e.g(r);
  } catch (...) {
  }
}
