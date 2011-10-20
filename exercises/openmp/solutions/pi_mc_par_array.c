/*

NAME:
   Pi_mc:  PI Monte Carlo

Purpose:
   This program uses a Monte Carlo algorithm to compute PI as an
   example of how random number generators are used to solve problems.
   Note that if your goal is to find digits of pi, there are much 
   better algorithms you could use.

Usage:
   To keep the program as simple as possible, you must edit the file
   and change the value of num_trials to change the number of samples
   used.  Then compile and run the program.

Algorithm:
   The basic idea behind the algorithm is easy to visualize.  Draw a 
   square on a wall.  Inside the square, draw a circle.  Now randomly throw 
   darts at the wall.  some darts will land inside the square.  Of those, 
   some will fall inside the circle.   The probability of landing inside
   the circle or the square is proportional to their areas.

   We can use a random number generator to "throw the darts" and count
   how many "darts" fall inside the square and how many inside the 
   cicle.  Dividing these two numbers gives us the ratio of their areas
   and from that we can compute pi.

Algorithm details:
   To turn this into code, I need a bit more detail.  Assume the circle
   is centered inside the square.  the circle will have a radius of r and 
   each side of the square will be of area 2*r (i.e. the diameter of the
   circle).  

       A(circle) = pi * r^2
       A(square) = (2*r)*(2*r) = 4*r^2

       ratio = A(circle)/A(square) = pi/4

   Since the probability (P) of a dart falling inside a figure (i.e. the square 
   or the circle) is proportional to the area, we have

       ratio = P(circle)/P(square) = pi/4

   If I throw N darts as computed by random numbers evenly distributed 
   over the area of the square

      P(sqaure) = N/N    .... i.e. every dart lands in the square
      P(circle) = N(circle)/N

      ratio = (N(circle)/N)/(N/N)  = N(circle)/N

   Hence, to find the area, I compute N random "darts" and count how many fall
   inside the circle.  The equation for a circle is

      x^2 + y^2 = r^2 

   So I randomly compute "x" and "y" evenly distributed from -r to r and 
   count the "dart" as falling inside the cicle if

      x^2 + y^2 < or = r

Supporting functions:
   For maximum portability, this file includes a very simple random number
   generator.  This is not a high quality generator and should not be used
   for serious work.

   The Generator is a linear congruential generator with constants selected
   to yield decent results for sequences with fewer than 2^28 numbers.  The
   pseudo random sequence is seeded with a range

       void seed(lower_limit, higher_limit)
   
   and then subsequent calls to the random number generator generates values
   in the sequence:

       double random()

Results:  
   Remember, our goal is to demonstrate a simple monte carlo algorithm, 
   not compute pi.  But just for the record, here are some results (Intel compiler
   version 10.0, Windows XP, core duo laptop)

       100        3.160000
       1000       3.148000
       10000      3.154000
       100000     3.139920
       1000000    3.141456
       10000000   3.141590
       100000000  3.141581

   As a point of reference, the first 7 digits of the true value of pi 
   is 3.141592 


History: 
   Written by Tim Mattson, 9/2007.

*/
#include <stdio.h>
#include <omp.h>

//**********************************************************
// Pseudo random number generator:
//     double random
//     void seed (lower_limit, higher_limit)
//**********************************************************

static unsigned long long MULTIPLIER  = 764261123;
static unsigned long long PMOD        = 2147483647;
static unsigned long long mult_n;
#define MAX_THREADS 128
static unsigned long pseed[MAX_THREADS];


// return a random integer for the thread
unsigned long urandom(int thread)
{
    unsigned long random_next = (unsigned long)((mult_n * pseed[thread]) % PMOD);
    pseed[thread] = random_next;
    return random_next;
}

// return a uniformly distributed random number on (0,1] for
// the thread.
double drandom(int thread)
{
    unsigned long random_next = urandom(thread);
    double ret_val = (double)random_next / (double)PMOD;
    return ret_val;
}


// Initialize RNG for n threads
void random_init(int nthreads, unsigned long in_seed)
{
    int i;
    if (in_seed == 0)
	in_seed = 1;
    pseed[0] = in_seed;
    mult_n = MULTIPLIER;
    for (i = 1; i < nthreads; ++i)
    {
	in_seed = (unsigned long)((MULTIPLIER * in_seed) % PMOD);
	pseed[i] = in_seed;
	mult_n = (mult_n * MULTIPLIER) % PMOD;
    }
}


//**********************************************************
// end of pseudo random generator code.
//**********************************************************

// 
// The monte carlo pi program
//

static double rand_low, rand_high;
void seed(double low, double high)
{
   int nthreads;
#pragma omp parallel
#pragma omp master
   nthreads = omp_get_num_threads();
   
   random_init(nthreads, 1234);
   rand_low = low;
   rand_high = high;
}

double random()
{
   double d = drandom(omp_get_thread_num());
   return d * (rand_high - rand_low) + rand_low;
}

static long num_trials = 1000000;

int main ()
{
   long i;  long Ncirc = 0;
   double pi, time;
   double r = 1.0;   // radius of circle. Side of square is 2*r 

   time = omp_get_wtime();
   seed(-r, r);  // The circle and square are centered at the origin

#pragma omp parallel for reduction(+:Ncirc)
   for(i=0;i<num_trials; i++)
   {
      double x, y, test;
      x = random();
      y = random();

      test = x*x + y*y;

      if (test <= r*r) Ncirc++;
    }

    pi = 4.0 * ((double)Ncirc/(double)num_trials);

    printf("\n %d trials, pi is %f \n",num_trials, pi);
    printf(" in %f seconds\n",omp_get_wtime()-time);

    return 0;
}
	  





