#include "multiply.h"

// matrix multiply routine

void multiply_d(double a[][DIM], double  b[][DIM], double  c[][DIM])
{
	int i,j,k;
	double temp;
	for(i=0;i<NUM;i++) {
	    for(k=0;k<NUM;k++) {
		  for(j=0;j<NUM;j++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
		  		}
		}
	}
}

