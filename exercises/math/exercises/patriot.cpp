#include<cstdio>


int main() {
   float tenth=0.1f;
   float t=0;
   long long n=0;
   while(n<1000000) {
     t+=0.1f;
     ++n;
     if (n<21 || n%36000==0) printf("%d %f %a\n",n,t,t);
   }

   float count = float(60*60*100*10);
   printf("\n\n%f %f %a\n\n",count,float(count*tenth),float(count*tenth));

   return 0;
}
