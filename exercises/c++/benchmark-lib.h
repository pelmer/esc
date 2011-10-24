/*
   Simple/naive measurements to give a rough idea of the relative
   cost of facilities related to OOP.
   This could be fooled/foiled by clever optimizers and by
   cache effects.
   Run at least three times to ensure that results are repeatable.
   Tests:
       virtual function
       global function called indirectly
       nonvirtual member function
       global function
       inline member function
       macro
       1st branch of MI
       2nd branch of MI
       call through virtual base
       call of virtual base function
       dynamic cast
       two-level dynamic cast
       typeid()
       call through pointer to member
       call-by-reference
       call-by-value
       pass as pointer to function
       pass as function object
    not yet:
        co-variant return
    The cost of the loop is not measurable at this precision:
    see inline tests
    By default do 1000000 iterations to cout
    1st optional argument: number of iterations
    2nd optional argument: target file name
 */
//int body(int i) { return i*(i+1)*(i+2); }
class X {
    int x;
    static int st;
public:
    virtual void f(int a);
    void g(int a);
    static void h(int a);
    void k(int i) { x+=i; }    // inline
};
struct S {
    int x;
};
int glob = 0;
extern void f(S* p, int a);
extern void g(S* p, int a);
extern void h(int a);
typedef void (*PF)(S* p, int a);
PF p[10] = { g , f };
// inline void k(S* p, i) { p->x+=i; }
#define K(p,i) ((p)->x+=(i))

struct T {
    const char* s;
    double t;
    T(const char* ss, double tt) : s(ss), t(tt) {}
    T() : s(0), t(0) {}
};
struct A {
    int x;
    virtual void f(int) = 0;
    void g(int);
};

struct B {
    int xx;
    virtual void ff(int) = 0;
    void gg(int);
};
struct C : A, B {
    void f(int);
    void ff(int);
};
struct CC : A, B {
    void f(int);
    void ff(int);
};
void  A::g(int i)    { x  += i; }
void  B::gg(int i)   { xx += i; }
void  C::f(int i)    { x  += i; }
void  C::ff(int i)   { xx += i; }
void  CC::f(int i)   { x  += i; }
void  CC::ff(int i)  { xx += i; }
template<class T, class T2> inline T* cast(T*, T2* q)
{
    glob++;
    return dynamic_cast<T*>(q);
}
struct C2 : virtual A {    // note: virtual base
};
struct C3 : virtual A {
};
struct D : C2, C3 {    // note: virtual base
     void f(int);
};
void D::f(int i)    { x+=i; }
struct P {
     int x;
     int y;
};
void by_ref(P& a) { a.x++; a.y++; }
void by_val(P a) { a.x++; a.y++; }
template<class F, class V> inline void oper(F f, V val) { f(val); }
struct FO {
     void operator () (int i) { glob += i; }
};
