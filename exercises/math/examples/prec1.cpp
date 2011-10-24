#include<cmath>
#include<numeric>
#include<cstdio>
#include<limits>




int main(int npar, char * []) {

  constexpr auto maxf = std::numeric_limits<float>::max();
  

  float a = 100.f+3.f/7.f;
  // if (npar>2) a=nextafterf(a,-1.f);
  float b = 4.f/7.f;
  if (npar>2) b=nextafterf(b,-maxf);


  float s =a+b;
  float z = s-a;
  float t = b-z;
  
  printf("%g %g %g %g %g\n",a,b,s,z,t);
  printf("%a %a %a %a %a\n",a,b,s,z,t);
  printf("%g %a\n",nextafterf(s,maxf),nextafterf(s,maxf));
  printf("%g %a\n\n\n",nextafterf(s,maxf)-s,nextafterf(s,maxf)-s);


  float pi2=2.f*std::acos(-1.f);
  if (npar>2) pi2=nextafterf(pi2,-maxf);
  s = pi2+1.f;
  z = s-pi2;
  t = 1.f-z;
  float c1 = std::cos(s);
  float c2 = c1 - t*std::sin(s);
  printf("%g %g %g %g %g %g %g\n",pi2,s,z,t,c1,c2,std::cos(1.f));
  printf("%a %a %a %a %a %a %a\n\n",pi2,s,z,t,c1,c2,std::cos(1.f));
  t =s;
  s = pi2+t;
  z = s-t;
  t = pi2-z;
  c1 = std::cos(s);
  c2 = c1 - t*std::sin(s);
  printf("%g %g %g %g %g %g %g\n",pi2,s,z,t,c1,c2,std::cos(1.f));
  printf("%a %a %a %a %a %a %a\n",pi2,s,z,t,c1,c2,std::cos(1.f));
  return 0;

}
