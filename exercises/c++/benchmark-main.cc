// --------------------------------------------------------------
#include <stdlib.h> // or <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>     // or <ctime>
#include <vector>
#include <typeinfo>

#include "benchmark-lib.h"

using namespace std;
template<class T> inline T* ti(T* p)
{
  if (typeid(p) == typeid(int*))
    p++;
  return p;
}
int main(int argc, char* argv[])
{
  long i;    // loop variable here for the benefit of non-conforming compilers
  long n = (1 < argc) ? atoi(argv[1]) : 100000000;   // number of iterations
  ofstream target;
  ostream* op = &cout;
  if (2 < argc) { // place output in file
    target.open(argv[2]);
    op = &target;
  }
  ostream& out = *op;
  // output command for documentation:
  for (i = 0; i < argc; ++i)
    out << argv[i] << " ";
  out << endl;
  X* px = new X;
  X  x;
  S* ps = new S;
  S  s;
  vector<T> v;
  clock_t t = clock();
  if (t == clock_t(-1)) {
    cerr << "sorry, no clock" << endl;
    exit(1);
  }
  for (i = 0; i < n; i++)
    px->f(1);
  v.push_back(T("virtual px->f(1)             ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    p[1](ps, 1);
  v.push_back(T("ptr-to-fct p[1](ps,1) ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    x.f(1);
  v.push_back(T("virtual x.f(1)        ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    p[1](&s, 1);
  v.push_back(T("ptr-to-fct p[1](&s,1) ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    px->g(1);
  v.push_back(T("member px->g(1)       ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    g(ps, 1);
  v.push_back(T("global g(ps,1)        ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    x.g(1);
  v.push_back(T("member x.g(1)         ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    g(&s, 1);
  v.push_back(T("global g(&s,1)        ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    X::h(1);
  v.push_back(T("static X::h(1)        ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    h(1);
  v.push_back(T("global h(1)           ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    px->k(1);
  v.push_back(T("inline px->k(1)       ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    K(ps, 1);
  v.push_back(T("macro K(ps,1)         ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    x.k(1);
  v.push_back(T("inline x.k(1)               ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    K(&s, 1);
  v.push_back(T("macro K(&s,1)               ", clock() - t));
  C* pc = new C;
  A* pa = pc;
  B* pb = pc;
  t = clock();
  for (i = 0; i < n; i++)
    pc->g(i);
  v.push_back(T("base1 member pc->g(i)       ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    pc->gg(i);
  v.push_back(T("base2 member pc->gg(i)      ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    pa->f(i);
  v.push_back(T("base1 virtual pa->f(i)      ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    pb->ff(i);
  v.push_back(T("base2 virtual pb->ff(i)     ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pa, pc);
  v.push_back(T("base1 down-cast cast(pa,pc)  ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pb, pc);
  v.push_back(T("base2 down-cast cast(pb,pc)  ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pc, pa);
  v.push_back(T("base1 up-cast cast(pc,pa)    ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pc, pb);
  v.push_back(T("base2 up-cast cast(pc,pb)    ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pb, pa);
  v.push_back(T("base2 cross-cast cast(pb,pa) ", clock() - t));
  CC* pcc = new CC;
  pa = pcc;
  pb = pcc;
  t = clock();
  for (i = 0; i < n; i++)
    cast(pa, pcc);
  v.push_back(T("base1 down-cast2 cast(pa,pcc)", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pb, pcc);
  v.push_back(T("base2 down-cast  cast(pb,pcc)", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pcc, pa);
  v.push_back(T("base1 up-cast cast(pcc,pa)   ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pcc, pb);
  v.push_back(T("base2 up-cast2 cast(pcc,pb)  ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pb, pa);
  v.push_back(T("base2 cross-cast2 cast(pa,pb)", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pa, pb);
  v.push_back(T("base1 cross-cast2 cast(pb,pa)", clock() - t));
  D* pd = new D;
  pa = pd;
  t = clock();
  for (i = 0; i < n; i++)
    pd->g(i);
  v.push_back(T("vbase member pd->gg(i)      ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    pa->f(i);
  v.push_back(T("vbase virtual pa->f(i)      ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pa, pd);
  v.push_back(T("vbase down-cast cast(pa,pd)  ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    cast(pd, pa);
  v.push_back(T("vbase up-cast cast(pd,pa)     ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    ti(pa);
  v.push_back(T("vbase typeid(pa)             ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    ti(pd);
  v.push_back(T("vbase typeid(pd)             ", clock() - t));
  void (A::* pmf)(int) = &A::f;       // virtual
  t = clock();
  for (i = 0; i < n; i++)
    (pa->*pmf)(i);
  v.push_back(T("pmf virtual (pa->*pmf)(i)   ", clock() - t));
  pmf = &A::g;                        // non virtual
  t = clock();
  for (i = 0; i < n; i++)
    (pa->*pmf)(i);
  v.push_back(T("pmf (pa->*pmf)(i)           ", clock() - t));
  P pp;
  t = clock();
  for (i = 0; i < n; i++)
    by_ref(pp);
  v.push_back(T("call by_ref(pp)             ", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    by_val(pp);
  v.push_back(T("call by_val(pp)             ", clock() - t));
  FO fct;
  t = clock();
  for (i = 0; i < n; i++)
    oper(h, glob);
  v.push_back(T("call ptr-to-fct oper(h,glob)", clock() - t));
  t = clock();
  for (i = 0; i < n; i++)
    oper(fct, glob);
  v.push_back(T("call fct-obj oper(fct,glob) ", clock() - t));
  if (clock() == clock_t(-1)) {
    cerr << "sorry, clock overflow" <<endl;
    exit(2);
  }
  out << endl;
  for (i = 0; i < v.size(); i++)
    out << v[i].s << " :\t"
	<< v[i].t * (double(1000000)/n)/CLOCKS_PER_SEC
	<< " ms" << endl;
  if (argc < 2) {    // if output is going to cout
    cout << "press any character to finish" << endl;
    char c;
    cin  >> c;     // to placate Windows console mode
  }
  return 0;
}
int  X::st = 0;
void X::f(int a)    { x += a; }
void X::g(int a)    { x += a; }
void X::h(int a)    { st += a; }
void f(S* p, int a) { p->x += a; }
void g(S* p, int a) { p->x += a; }
void h(int a)       { glob += a; }
