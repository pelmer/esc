#include<cstdio>
#include<cmath>
#include<limits>
#include <fenv.h>

int main() {

  // c++  -std=gnu++11 round.cpp -frounding-math
  #pragma STDC FENV_ACCESS ON 

  // fesetround(FE_DOWNWARD);
  // fesetround(FE_UPWARD);
  
  constexpr auto maxf = std::numeric_limits<float>::max();
  


  float a=0.1f;
  float a1=nextafterf(a,-maxf);
  float a2=nextafterf(a,maxf);
  float b1=0x1.8p-4;   // 0x2.0p-4;
  float b2=0x1.9p-4;   // 0x2.0p-4;
  float b3=0x1.ap-4;   // 0x2.0p-4;

  float s0=0., s1=0., s2=0., s3=0.;

  for (int i=0; i!=1000000; ++i) {
    s0+=a; s1+=b1; s2+=b2;s3+=b3;
  }

  printf("%g %a\n",a,a);
  printf("%g %a\n",a1,a1);
  printf("%g %a\n\n",a2,a2);
  printf("%g %a\n",2*a,2*a);
  printf("%g %a\n",3*a,3*a);
  printf("%g %a\n",4*a,4*a);
  printf("%g %a\n",5*a,5*a);
  printf("%g %a\n\n",6*a,6*a);
  printf("%g %a\n",3*a+a,3*a+a);
  printf("%g %a\n\n",5*a+a,5*a+a);
  printf("%g %a\n",0.9f,0.9f);
  printf("%g %a\n",0.6f,0.6f);
  printf("%g %a\n",0.3f,0.3f);
  printf("%g %a\n",0.9f-0.6f,0.9f-0.6f);
  printf("%g %a\n",0.9f/3.f,0.9f/3.f);
  printf("%g %a\n",0.6f/2.f,0.6f/2.f);
  printf("%g %a\n",0.9f*(1.f/3.f),0.9f*(1.f/3.f));
  printf("%g %a\n\n",0.6f*0.5f,0.6f*0.5f);

  printf("%g %a\n",b1,b1);
  printf("%g %a\n",b2,b2);
  printf("%g %a\n\n",b3,b3);


  printf("%g %a\n",s0,s0);
  printf("%g %a\n",s1,s1);
  printf("%g %a\n",s2,s2);
  printf("%g %a\n\n\n",s3,s3);


  //  float x = 0x1.000002p-1;
  //float x = 0x1.000004p-1;
  float x = 0x1.000006p-1;
  printf("x   %g %a\n",x,x);
  printf("x+x %g %a\n",x+x,x+x);
  float y=x;
  for (int i=0; i<5; i++) {
    y*=2;
    float z = y+x;
    double d = double(y)+double(x);
    printf("y %g %a\n",y,y);
    printf("y+x %g %a\n",z,z);
    printf("y+x %g %a\n",d,d);
    printf("y+x %g %a\n",float(d),float(d));
  }



  return 0;
}
