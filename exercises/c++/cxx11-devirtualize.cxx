
#ifdef WITH_FINAL
# define FINAL final
# define NOFINAL
#else
# define FINAL
# define NOFINAL
#endif

struct A {
  virtual ~A(){}
  virtual int a() const FINAL   { return m_a; }
          int b() const NOFINAL { return m_a; }
  int geta() const;
  int getb() const;

  int m_a;
};
int A::geta() const { return a();}
int A::getb() const { return b();}

