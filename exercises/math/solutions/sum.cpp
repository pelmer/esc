//  Time-stamp: "Modified on 14-February-2011 at 15:10:31 by jearnold on lxplus446.cern.ch"
//
//  CERN openlab Computer Architecture and Performance Tuning Workshop - Winter 2011
//  --------------------------------------------------------------------------------
//
//  Sum a list of pseudo-random numbers using various methods. 
//  The array of numbers to sum is generated in two different way:
//     NOTNASTY: as they come between -max and max
//     "NASTY": large numebrs for even values of the subscript, small for even values, alternating positive and negative i.e.
//     0: large positive, 1: small positive, 2: large negative, 2: small negative 
// The methods used are
//      1.  simply sum the values in their array order.
//      2.  a highly accuration summation scheme.
//      3.  same as #2 except that the code is modified to allow vectorization to take place
//      4.  same as #1 except that the loop is split into 4 subloops; basically
//          a reduction.  However, this scheme intereacts with how the numbers
//          are created (with the "nasty" option).
//      5.  same as #1 except that the loop is split into 5 subloops; basically
//          a reduction.
//      6.  use an OpenMP reduction.
//      7.  sort the values by their value before adding them.
//      9.  sort the values by their magnitude before adding them.
//
//  The point of the exercise is to demonstrate the variation of results possible.
//
// modified to follow c++11 standard
// modified to test float and double (eventually long double and double128)
// modified to add vectorization of KS
// modified to add non-nasty filling scheme
// modified to add precision timing
// by Vin for the ECS11 school in Berinoro 

#include <cmath>
#include <omp.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <random>
#include <numeric>
#include <algorithm>
#include <x86intrin.h>
inline volatile unsigned long long rdtsc() {
  return __rdtsc();
}

#if !defined( DEBUG_IT )
#define DEBUG_IT 0
#endif

//  N_VALUES should be a multiple of both 4 and 5 but this is not enforced.
#if !defined( N_VALUES )
#define N_VALUES 100000
#endif

#if !defined( ADJUSTMENT_FACTOR )
#define ADJUSTMENT_FACTOR 0.
#endif

#if !defined( MAX_VALUE )
#define MAX_VALUE 1.
#endif

void noprint(...){}

#ifdef NOPRINT
#define PRINT noprint
#else
#define PRINT printf
#endif




template<typename T>
inline T KahanSummation( const T a[],
			 const int N , T t=0)
{
  T s = a[ 0 ];			//  sum
  //  correction is t
  for( int i = 1; i < N; i++ ) {
    T y = a[ i ] - t;		//  next term "plus" correction
    T z = s + y;		//  add to accumulated sum
    t = ( z - s ) - y;		//  t <- -( low part of y )
    s = z;				//  update sum
  }
  return s;
}


template<typename T>
inline T VectorKahanSummation( const T a[],
			       const int N, const int stride )
{
  const int loops = N/stride;
  const int tail =  N - stride*loops;
 
  T s[stride+1]; ::memcpy(s,a,sizeof(T)*stride);			//  sum
  T t[stride];   for (int j=0; j!=stride; ++j) t[j]=0;		//  correction
  for( int i = 1; i < loops; i++ ) {
    int is=stride*i;
    T y[stride];
    T z[stride]; 
    for (int j=0; j!=stride; ++j) {
      y[j] = a[is+j ] - t[j];		//  next term "plus" correction
      z[j] = s[j] + y[j];		//  add to accumulated sum
      t[j] = ( z[j] - s[j] ) - y[j];		//  t <- -( low part of y )
      s[j] = z[j];				//  update sum
    }
  }
  T tt=0;  for (int j=0; j!=stride; ++j) tt+=t[j];
  s[stride] =  tail>0 ? KahanSummation(a+stride*loops,tail) : 0;
  return KahanSummation(s,stride+1,tt);
}

//  Default initialization used for random.
std::mt19937 eng;


template<typename T>
T compute(volatile double times[]) {

  std::uniform_real_distribution<T> rgen(0.,MAX_VALUE); 


  PRINT("\n\ntesting sum for %d numbers\n", N_VALUES);
  T tot=0;

  T array[ N_VALUES + 4 ];


#ifdef NOTNASTY
  for ( int i = 0; i < N_VALUES; i++ )  array[ i ] = T(MAX_VALUE)-T(2)*rgen(eng);
#else
  // nasty machinery...
  for ( int i = 0; i < N_VALUES; i++ ) {
    T temp = rgen(eng);
    if ( !( i & 1 ) ) {				//  for even values of the subscript.
      array[ i ] = 100.0 * temp;
      if ( !( i & 2 ) ) array[ i ] = -array[ i ]; //  for every other even value, negate it.
    } else {					//  for odd values of the subscript.
      array[ i ] = 0.001 * temp;
      if ( i & 2 ) array[ i ] = -array[ i ];	//  for every other odd value, negate it.
    }
  }
#endif
 
  if ( DEBUG_IT ) {
    T x, y, z;
    x = y = array[ 0 ];
    z = std::abs( x );
    for ( int i = 1; i < N_VALUES; i++ ) {
      if ( array[ i ] < x ) x = array[ i ];
      if ( array[ i ] > y ) y = array[ i ];
      if ( std::abs( array[ i ] ) < z ) z = std::abs( array[ i ] );
    }
    PRINT( "min = %20.17f  max = %20.17f  smallest abs = %20.17f\n", x, y, z );
    for ( int i = 0; i < 8; i++ ) { PRINT( "value %d = %20.17f\n", i, array[ i ] ); }
  }


  //
  //  Sum the numbers by simplying adding them in order.
  //
  {
    auto volatile st = rdtsc();
    T sum = 0;
    for ( int i = 0; i < N_VALUES; i++ ) {
      sum += array[ i ];
    }
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[0]+=et;
    PRINT( "sum 0 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }

  //
  //  Sum the numbers by simplying adding them in order.
  //
  {
    auto volatile st = rdtsc();
    T sum = 0;
    for ( int i = 0; i < N_VALUES; i++ ) {
      sum += array[ i ];
    }
    sum -= T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[1]+=et;
    PRINT( "sum 1 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }
  //
  //  Sum the numbers by using a highly accuration summation scheme.
  //
  {
    auto volatile st = rdtsc();
    T sum = KahanSummation( array, N_VALUES ) - T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[2]+=et;
    PRINT( "sum 2 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }
 
 
  //
  //  Sum the numbers by using a highly accuration summation scheme (vectorized)
  //
  {
    auto volatile st = rdtsc();
    T sum = VectorKahanSummation( array, N_VALUES, 16 ) - T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[3]+=et;
    PRINT( "sum 3 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }


  //
  //  Sum the numbers by decomposing the addition loop into 4 separate pieces.
  //
  {
    auto volatile st = rdtsc();
    T sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;    
    for ( int i = 0; i < N_VALUES; i += 4 ) {
      sum0 += array[ i ];
      sum1 += array[ i + 1 ];
      sum2 += array[ i + 2 ];
      sum3 += array[ i + 3 ];
    }
    T sum = sum0 + sum1 + sum2 + sum3 - T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[4]+=et;
    PRINT( "sum 4 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }
  
  //
  //  Sum the numbers by decomposing the addition loop into 5 separate pieces.
  //
  {
    auto volatile st = rdtsc();
    T sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    for ( int i = 0; i < N_VALUES; i += 5 ) {
      sum0 += array[ i ];
      sum1 += array[ i + 1 ];
      sum2 += array[ i + 2 ];
      sum3 += array[ i + 3 ];
      sum4 += array[ i + 4 ];
    }
    T  sum = sum0 + sum1 + sum2 + sum3 + sum4 - T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[5]+=et;
    PRINT( "sum 5 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }
  
  PRINT( "Maximum number of threads is %d\n", omp_get_max_threads() );
  
  //
  //  Sum the numbers by using an OpenMP reduction.
  //
  {
    auto volatile st = rdtsc();
    T sum = 0;
#pragma omp parallel for reduction(+:sum) schedule(static)
    for ( int i = 0; i < N_VALUES; i++ ) {
      sum += array[ i ];
    }
    sum -= ADJUSTMENT_FACTOR;
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[6]+=et;
    PRINT( "sum 6 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }
  
  //
  //  Sort the values by increasing value and then add them.
  //
  {
    auto volatile st = rdtsc();
    std::sort(array, array+N_VALUES);
    T sum = 0.0;
    for ( int i = 0; i < N_VALUES; i++ ) {
      sum += array[ i ];
    }
    sum -= T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[7]+=et;
    PRINT( "sum 7 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }

  
  //
  //  Sort the values by increasing absolute value and then add them.
  //
  {
    auto volatile st = rdtsc();
    std::sort(array, array+N_VALUES,[](T x, T y){return std::abs(x)<std::abs(y);});
    T sum = 0.0;
    for ( int i = 0; i < N_VALUES; i++ ) {
      sum += array[ i ];
    }
    sum -= T(ADJUSTMENT_FACTOR);
    tot+=sum;
    auto volatile et = rdtsc() -st;
    times[8]+=et;
    PRINT( "sum 8 = %20.17f (%13.13a) %d\n", sum, sum, et );
  }
 
  return tot;
}


int main() {

#ifdef NOPRINT
  volatile float ftot=0;
  volatile float dtot=0;
  {
    volatile double times[9]={0};
    for (int i=0; i!=100; ++i) ftot +=compute<float>(times);
    for ( auto x : times) printf("| %g ",x);
    printf(" |\n\n");
  }

  {
    volatile double times[9]={0};
    for (int i=0; i!=100; ++i) dtot +=compute<double>(times);
    for ( auto x : times) printf("| %g ",x);
    printf(" |\n\n");
  }
#else
  volatile double times[9];
  volatile float ftot=0;
  volatile float dtot=0;
  ftot +=compute<float>(times);
  dtot +=compute<double>(times);
#endif

  return ftot+dtot;
}
