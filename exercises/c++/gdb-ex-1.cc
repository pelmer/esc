#include <stdio.h>

inline int inline_f ( int x ) { return x*x; }

int f( int x ) { return x*x; }

int main(int, char **) 
{
  int y, x = 5, z = 6; 
  printf("y= %d\n", y);
  y = f(x); 
  printf("y= %d\n", y);
  y = inline_f(z); 
  printf("y= %d\n", y);

  return 0;
}

