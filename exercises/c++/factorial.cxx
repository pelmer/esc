///////////////////////////////////////////////////////////////////////////////
// a simple template meta-programming example

/// a metafunction to compute n!
template<int N>
class Factorial
{
public:
  static const int value = N * Factorial<N-1>::value;
};

/// specialization to handle boundary conditions
template<>
class Factorial<1>
{
public:
  static const int value = 1;
};



///////////////////////////////////////////////////////////////////////////////
// another example
// Given a forward declaration:
template <int Dim, class T>
struct dot_class;
// a specialized base case for recursion:
template <class T>
struct dot_class<1,T> {
    static inline T dot(const T* a, const T* b)
      { return *a * *b; }
};
// the recursive template:
template <int Dim, class T>
struct dot_class {
    static inline T dot(const T* a, const T* b)
      { return dot_class<Dim-1,T>::dot(a+1,b+1) +
               *a * *b; }
  };
// ... and some syntactic sugar:
template <int Dim, class T>
inline T dot(const T* a, const T* b)
    { return dot_class<Dim,T>::dot(a, b); }


#include <stdio.h>
int main(int /*argc*/, char** /*argv*/)
{
  const int ndims = 3;
  const double a[ndims] = {1.,2.,1.};
  const double b[ndims] = {1.,2.,1.};

  {
    // Then
    int product = dot<ndims>(a, b);
    printf("tmpl-prod: %i\n", product);
  }

  {
    // results in the same (near-)optimal code as
    int product = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    printf("hand-prod: %i\n", product);
  }

  // factorial
  {
    printf(" 3! = %i\n", Factorial<3>::value);
    printf("12! = %i\n", Factorial<12>::value);
  }
  return 0;
}
