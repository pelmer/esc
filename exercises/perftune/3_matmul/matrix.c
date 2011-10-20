//  Simple minded matrix multiply
#include <stdio.h>
#include <time.h>
#include <ia32intrin.h>
#include "multiply.h"

typedef unsigned __int64 UINT64;


static double  a[DIM][DIM], b[DIM][DIM], c[DIM][DIM];


//routine to initialize an array with data
void init_arr(double row, double col, double off, double a[][DIM])
{
	int i,j;

	for (i=0; i< NUM;i++) {
		for (j=0; j<NUM;j++) {
			a[i][j] = row*i+col*j+off;
		}
	}
}


// routine to print out contents of small arrays
void print_arr(char * name, double array[][DIM])
{
	int i,j;
	
	printf("\n%s\n", name);
	for (i=0;i<NUM;i++){
		for (j=0;j<NUM;j++) {
			printf("%g\t",array[i][j]);
		}
		printf("\n");
	}
}


main()
{
	long long start, stop;
	int num;

// initialize the arrays with data
	init_arr(3,-2,1,a);
	init_arr(-2,1,3,b);

	//start timing the matrix multiply code
	printf("Matrix size: %d x %d\n", NUM, NUM);
	start = _rdtsc();
	multiply_d(a,b,c);
	stop = _rdtsc();

// print simple test case of data to be sure multiplication is correct
	if (NUM < 5) {
		print_arr("a", a);
		print_arr("b", b);
		print_arr("c", c);
	}
	
// print elapsed time
	printf("Elapsed time = %.2lf billion cycles\n", (double)(stop - start)/1e9 );
	printf("fraction of theoretical throughput limit = %.2f%%\n",(double)(100.*NUM*NUM*NUM/(2.*(stop-start))));

}

