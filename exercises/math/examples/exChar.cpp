#include <x86intrin.h>
#include <string>
#include <iostream>


union UI4 {
  UI4(){}
  UI4(__m128i i) : si(i){}

  __m128 v;
  __m128i si;
  unsigned int u[4];
  unsigned char uc[16];
  char c[16];
};

inline void printC(UI4 c) {
  for (int i=0;i!=16; ++i)
    std::cout << c.c[i];
  std::cout << std::endl;
}


inline bool operator==(UI4 rh, UI4 lh) {
  return  _mm_movemask_epi8(_mm_cmpeq_epi8(rh.si,lh.si))==0xffff;
}

// inverse lex compare....
inline bool operator<(UI4 rh, UI4 lh) {
  int ls = _mm_movemask_epi8(_mm_cmplt_epi8(rh.si,lh.si));
  int ge = _mm_movemask_epi8(_mm_cmplt_epi8(lh.si,rh.si));
  return ls>ge;
}


int main() {

 
  UI4 a = _mm_loadu_si128((const __m128i*)"abcd1234qwer5678");
  UI4 b = _mm_loadu_si128((const __m128i*)"abcd1234qwer5678");
  UI4 c = _mm_loadu_si128((const __m128i*)"abcd1235qwer5678");
  UI4 d = _mm_loadu_si128((const __m128i*)"abcf1233qwer5678");
  UI4 z = _mm_loadu_si128((const __m128i*)"zzzzzzzzqwer5671");
  printC (a);
  printC (b);
  printC (c);
  printC (d);
  std::cout << (a==b) << " " << (a<b) << std::endl;
  std::cout << (a==c) << " " << (a<c)  << " " << (c<a) << std::endl;
  std::cout << (a==d) << " " << (a<d)  << " " << (d<a) << std::endl;
  std::cout << (z==d) << " " << (a<z)  << " " << (z<a) << std::endl;


  return 0;
}
