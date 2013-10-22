/*
**  PROGRAM: jacobi Solver
**
**  PURPOSE: Generate a non-singular, diagonally dominant  
**           square matrix (A) and non-zero vector (b) and solve 
**           the linear system
**
**               Ax = b
**
**           Use a jacobi iterative solver.  Report the error term
**           as the square root of the average squared difference 
**           between the input b vector and the one generated from
**           product of the input A matrix and the computed x vector.
**           
**  USAGE:   Right now, I hardwire the matrix ORDER. Later, I'll 
**           take them from the command line.
**
**  HISTORY: Written by Tim Mattson, October 2013
**           Debugging and clean up by Vincenzo Innocente, October 2013
*/
// #define DEBUG 1
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "frandom.h"

#define ORDER   10000
#define SEED    571
#define TOL     0.000001
#define TYPE    double 
#define kMAX    100

int main(int argc, char **argv)
{
   int i,j,k;
   TYPE *A, *b, *xnew, *xold, xval, sum, dif, error;
   TYPE conv; 
   double start_time, run_time;

   A    = (TYPE *)malloc(ORDER*ORDER*sizeof(TYPE));
   b    = (TYPE *)malloc(ORDER*sizeof(TYPE));
   xold = (TYPE *)malloc(ORDER*sizeof(TYPE));
   xnew = (TYPE *)malloc(ORDER*sizeof(TYPE));

   start_time = omp_get_wtime(); 
   // generate a diagonally dominate matrix and a nonzero b vector
   // This means we need to assure that the diagonal elements of A
   // are greater than the sum across the whole row

   s_l_random(SEED,1,1000000);

   for (i=0; i<ORDER; i++){
      sum = 0.0;
      for (j=0; j<ORDER; j++){
         xval = f_random();
         A[i*ORDER+j]  = xval;
         sum += xval;
      }
      b[i] = f_random() + TOL;
      A[i*ORDER+i] += 1.5*sum;
   }

   // Solve Ax=b using a jacobi iterative solver

   // 1. Guess a starting value for x (forced to be non-zero)
   for (i=0; i<ORDER; i++){
      xold[i] = f_random() + TOL;
   }

   // 2. Iteratively update estimates of x

   k = 0;
   conv = (TYPE)(kMAX);  // start with a large convergence value
   while (conv > TOL && k<kMAX){
     for (i=0; i<ORDER; i++){
       xval = (TYPE)0.0;
       // 2b. j loop over all j not equal to i
       for(j=0;j<i;j++){
         xval += A[i*ORDER+j] * xold[j];
       }
       for(j=i+1;j<ORDER;j++){
         xval += A[i*ORDER+j] * xold[j];
      }
       xnew[i]= (b[i] - xval)/A[i*ORDER+i];
     }
     // 3. test convergence and increment iteration count
     conv = 0.0;
     for(i=0;i<ORDER;i++){
       dif = xnew[i]-xold[i];
       conv += dif*dif;
     }
     k++;
     // 4. Copy x for next round
     for(j=0;j<ORDER;j++)
       xold[j] = xnew[j];
#ifdef DEBUG
     printf(" number of iterations %d, conv %f\n",k,conv);
#endif

   }

   // test results
   error = 0.0;
   for (i=0; i<ORDER; i++){
      sum = 0.0;
      for (j=0; j<ORDER; j++){
         sum +=  A[i*ORDER+j]*xnew[j];  
      }
#ifdef DEBUG
      printf(" %d sum = %f and b = %f \n",i,sum,b[i]);
#endif
      dif = b[i]-sum;
      error += dif*dif;
    }
    error = sqrt((error)/(TYPE)ORDER);

   run_time = omp_get_wtime() - start_time;

   printf(" Order %d solver, ave error = %f and %d iterations in %lf seconds\n",
                   ORDER,error,k,run_time);

   printf("\n all done \n");
}

