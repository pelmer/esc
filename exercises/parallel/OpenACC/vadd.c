//------------------------------------------------------------------------------
//
// Name:       vadd.c
// 
// Purpose:    Elementwise addition of two vectors (c = a + b).  
//             Uses OpenACC
//
//                   c = a + b
//
// HISTORY:    Written by Tim Mattson, Oct 2013
//             
//------------------------------------------------------------------------------

#include <stdio.h>
#define TOL    (0.001)   // tolerance used in floating point comparisons
#define LENGTH (1024)    // length of vectors a, b, and c
#define SMALL  (0.01)    // a small scale factor to keep a and b values small
extern double wtime();   // timer: returns seconds since time in the past

//void vadd(int n, const float *a, const flaot *b, float *restrict c)
void vadd(int n, const float *a, const float *b, float *c)
{
   int i;
   #pragma acc parallel loop
   for (i=0;i<n; i++)
       c[i] = a[i] + b[i];
}

int main(void)
{
   int i, count = LENGTH;
   double t0, rtime; 
   float a[LENGTH], b[LENGTH], c[LENGTH];
   for(i = 0; i < LENGTH; i++)
   {
      a[i]  = i * SMALL;
      b[i]  = i * SMALL * 2;
   }

   t0    = wtime();
   vadd(count, a, b, c);
   rtime = wtime() - t0;

   printf("\nThe vadd kernel ran in %lf seconds\n", rtime);

   // Test the results
   int correct = 0;
   float tmp;
   for(i = 0; i < count; i++) {
      tmp = a[i] + b[i];       // expected value for c[i]
      tmp -= c[i];           // compute errors
      if(tmp*tmp < TOL*TOL) {  // correct if square deviation is less 
         correct++;           //  than tolerance squared
      }
      else {
        printf( " tmp %f a %f b %f  c %f \n", tmp, a[i], b[i], c[i]);
      }
   }

   // summarize results
   printf( "vector add to find C = A+B:  %d out of %d results were correct.\n", 
      correct, count);
}
