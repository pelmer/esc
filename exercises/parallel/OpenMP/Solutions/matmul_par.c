/*
**  PROGRAM: Matrix Multiply
**
**  PURPOSE: This is a simple matrix multiply program. 
**           It will compute the product
**
**                C  = A * B
**
**           A and B are set to constant matrices so we
**           can make a quick test of the multiplication.
**
**  USAGE:   Right now, I hardwire the martix dimensions. 
**           later, I'll take them from the command line.
**
**  HISTORY: Written by Tim Mattson, Oct 6, 2013.
*/
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include <omp.h>

#define ORDER 1000
#define AVAL 3.0
#define BVAL 5.0
#define ZERO 0.0
#define TOL  0.001
#define MAX_THREADS 4

///////  utility functions //////////
void clear_mat(int Ndim, int Pdim, double *A)
{
   int i,j;
   for (i=0; i<Ndim; i++)
      for (j=0; j<Pdim; j++)
         *(A+(i*Pdim+j)) = ZERO;
}
void set_const_mat(int Ndim, int Pdim, double val, double *A)
{
   int i,j;
   for (i=0; i<Ndim; i++)
      for (j=0; j<Pdim; j++)
         *(A+(i*Pdim+j)) = val;
}
void test_results(int Ndim, int Pdim, int Mdim, int AA, int BB, double *C)
{
   int i,j;
   double cval, err, errsq = 0.0;
   cval = Pdim * AA * BB;
   for (i=0; i<Ndim; i++){
      for (j=0; j<Mdim; j++){
         err = *(C+i*Mdim+j) - cval;
         errsq += err * err;
      }
   }

   if (errsq > TOL) 
      printf("\n Errors in multiplication: %f",errsq);
   else
      printf("\n Hey, it worked");
}

///////  the matrix multiplication function //////////
void serial_mat_mult(int Ndim, int Mdim, int Pdim, 
                     double *A, double *B, double *C) 
{
   int i, j, k;
   double tmp;
   for (i=0; i<Ndim; i++){
      for (j=0; j<Mdim; j++){
         tmp = 0.0;
         for(k=0;k<Pdim;k++){
            /* C(i,j) = sum(over k) A(i,k) * B(k,j) */
            tmp += *(A+(i*Pdim+k)) *  *(B+(k*Mdim+j));
         }
         *(C+(i*Mdim+j)) = tmp;
      }
   }
}
void par_mat_mult(int Nthreads, int Ndim, int Mdim, int Pdim, 
                     double *A, double *B, double *C) 
{
   int i, j, k;
   double tmp;
   #pragma omp parallel private(tmp,j,k)  num_threads(Nthreads)
   {
      #pragma omp master
          printf(" par mat mult with %d threads\n",omp_get_num_threads());

      #pragma omp for 
      for (i=0; i<Ndim; i++){
         for (j=0; j<Mdim; j++){
            tmp = 0.0;
            for(k=0;k<Pdim;k++){
               /* C(i,j) = sum(over k) A(i,k) * B(k,j) */
               tmp += *(A+(i*Pdim+k)) *  *(B+(k*Mdim+j));
            }
            *(C+(i*Mdim+j)) = tmp;
         }
      }
   }
}


int main(int argc, char **argv)
{
   int Ndim, Pdim, Mdim;   /* A[N][P], B[P][M], C[N][M] */
   int i;
   double *A, *B, *C;
   double dN, mflops;
   double start_time, run_time;

   Ndim = ORDER;
   Pdim = ORDER;
   Mdim = ORDER;

   A = (double *)malloc(Ndim*Pdim*sizeof(double));
   B = (double *)malloc(Pdim*Mdim*sizeof(double));
   C = (double *)malloc(Ndim*Mdim*sizeof(double));

	/* Initialize matrices */

   set_const_mat(Ndim, Pdim, AVAL, A);
   set_const_mat(Pdim, Mdim, BVAL, B);
   clear_mat(Ndim, Mdim, C);

	/* Do the matrix product */

   start_time = omp_get_wtime(); 

   serial_mat_mult(Ndim, Mdim, Pdim, A, B, C);

   run_time = omp_get_wtime() - start_time;
   printf(" Order %d serial mult in %f seconds \n", ORDER, run_time);
   dN = (double)ORDER;
   mflops = 2.0 * dN * dN * dN/(1000000.0* run_time);
   printf(" Order %d serial mult at %f mflops\n", ORDER, mflops);

   for(i=1; i<=MAX_THREADS; i++){
      start_time = omp_get_wtime(); 

      par_mat_mult(i, Ndim, Mdim, Pdim, A, B, C); 

      run_time = omp_get_wtime() - start_time;
      printf(" Order %d multiplication in %f seconds \n", ORDER, run_time);
      dN = (double)ORDER;
      mflops = 2.0 * dN * dN * dN/(1000000.0* run_time);
      printf(" Order %d multiplication at %f mflops\n", ORDER, mflops);
   }

   printf("\n all done \n");
}
