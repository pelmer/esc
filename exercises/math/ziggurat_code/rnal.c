/*==========================================================================
 *  This code is Copyright (C) 2005, Jurgen A. Doornik.
 *  Permission to use this code for non-commercial purposes
 *  is hereby given, provided proper reference is made to:
 *		Doornik, J.A. (2005), "An Improved Ziggurat Method to Generate Normal
 *          Random Samples", mimeo, Nuffield College, University of Oxford,
 *			and www.doornik.com/research.
 *		or the published version when available.
 *	This reference is still required when using modified versions of the code.
 *  This notice should be maintained in modified versions of the code.
 *	No warranty is given regarding the correctness of this code.
 *==========================================================================*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "zigrandom.h"


/*-------------------------------------- NAL -------------------------------*/
#define NAL_C 128
static int s_aiNAL_q[NAL_C] =
{
	28, 32, 33, 36, 40, 43, 45, 47, 51, 53, 57, 59, 27, 37, 43, 54,
	28, 45, 60, 63, 49, 61, 52, 34, 63, 46, 34, 18, 47, 40, 36, 29,
	61, 57, 53, 51, 47, 43, 40, 37, 33, 31, 27, 25, 21, 19, 17, 14,
	11,  9,  8,  5, 54, 51, 49, 46, 44, 41, 39, 37, 35, 33, 31, 29,
	28, 26, 24, 23, 21, 20, 19, 18, 16, 15, 14, 13, 12, 12, 11, 10, 
	 9,  9,  8,  7,  7,  6,  6,  5,  5,  5,  4,  4,  4,  3,  3,  3,
	 3,  3,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,
	 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
};
static int s_aiNAL_a[NAL_C] =
{
	31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 27, 26, 25, 24,
	23, 22, 21, -1,	19, 19, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
	19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,
	 3,  2,  1,  0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
	11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0, 51, 50, 49, 48, 
	47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,
	31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
	15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0
};
static int s_aiNAL_y[NAL_C] =
{
	200,199,199,198,197,196,195,193,192,190,188,186,184,181,179,176,
	173,170,167,164,161,157,154,151,147,143,140,136,132,128,125,121,
	117,113,110,106,102, 98, 95, 91, 88, 84, 81, 77, 74, 71, 68, 64,
	 61, 59, 56, 53, 50, 48, 45, 43, 40, 38, 36, 34, 32, 30, 28, 27,
	 25, 23, 22, 20, 19, 18, 17, 15, 14, 13, 12, 11, 11, 10,  9,  8,
	  8,  7,  6,  6,  5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,
	  2,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
static int s_aiNAL_dy[NAL_C] = 			  /* y[i-1]-y[i]+1, where y[-1]=200 */
{
 	  1,  2,  1,  2,  2,  2,  2,  3,  2,  3,  3,  3,  3,  4,  3,  4,
	  4,  4,  4,  4,  4,  5,  4,  4,  5,  5,  4,  5,  5,  5,  4,  5,
	  5,  5,  4,  5,  5,  5,  4,  5,  4,  5,  4,  5,  4,  4,  4,  5,
	  4,  3,  4,  4,  4,  3,  4,  3,  4,  3,  3,  3,  3,  3,  3,  2,
	  3,  3,  2,  3,  2,  2,  2,  3,  2,  2,  2,  2,  1,  2,  2,  2,
	  1,  2,  2,  1,  2,  1,  2,  1,  1,  2,  1,  1,  2,  1,  1,  1,
	  1,  2,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,
	  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
};

static int ranNormalNALrest(int alias, int i, double u, int negative, double *pX)
{
	double v, x, t;

	if (alias)    	      					 	 	     /* do the tail */
	{
		x = 3.75 + 0.25 / u;
		t = 8.0 - 0.5 * x * x;									  /* -t */
		v = 1.861970434352886050 * u * u * DRanU();
	}
	else								/* do the wedge from interval i */
	{                         
		x = (u + i) / 32;
		t = -0.5 * x * x;
		v = 0.004996971959878404 * (s_aiNAL_y[i] + s_aiNAL_dy[i] * DRanU());
	}
	if (log(v) <= t)
	{	*pX = negative ? -x : x;
		return 1;
	}
	return 0;
}
double DRanNormalNALOrig()
{
	unsigned int iran;
	int k, i, s;
	double u, x;

	for (;;)
	{
		iran = IRanU();								   /* 1. U=.B1 B2 B3 B4 */
		s = iran & 0x80000000;		 		/* sign is most significant bit */
		i = (iran >> 24) & 0x7F;					/* i is next seven bits */
		k = (((iran >> 16) ^ iran) >> 2) & 0x3F;	  /* 3. K=B2^B4 K >>= 2 */

		if (k >= s_aiNAL_q[i])    	       	 	      /* 5. do alias method */
		{
			if (i != 19)									     
			{
				i = s_aiNAL_a[i];
				iran = (i << 24) | (iran & 0x00FFFFFF);/* set high byte to i*/
				/* iran=J has high bit 0, so J*2^{-32} is in [0,0.5) */
				/* multiply by 8 to get [0,4): 8*J*2^{-32}=J*2^{-29} */
				/* this will automatically be in box i: [i/32,(i+1)/32) */
				x = iran * 1.86264514923095703125e-9;			    /* X=8U */
				return s ? -x : x;
			}
			u = DRanU();		                     	 /* 11. do the tail */
		}
		else												 /* 7. criminal */
		{                         
			iran &= 0x7FFFFFFF;
			x = iran * 1.86264514923095703125e-9;				    /* X=8U */
			if (k >= s_aiNAL_dy[i])				/* 7. criminal test for A/R */			 
				return s ? -x : x;
			u = (iran & 0x00FFFFFF) * 5.9604644775390625e-8;
		}
		if (ranNormalNALrest(k >= s_aiNAL_q[i], i, u, s, &x))
			return x;
	}
}
double DRanNormalNAL()
{
	unsigned int iran;
	int k, i, s, alias;
	double u, x;

	for (;;)
	{
		iran = IRanU();
		s = iran & 0x80;	  /* sign is most significant bit of first byte */
		i = iran & 0x7F;				  /* i in 0..127 is next seven bits */
		k = (iran >> 8) & 0x3F;							   /* 3. K in 0..63 */
		alias = k >= s_aiNAL_q[i];			 /* yes: use alias instead of i */
		
		u = DRanU();
		
		if (alias && i != 19)          /* do alias method (but not on tail) */
		{	
			x = (u + s_aiNAL_a[i]) / 32;	    /* sample from interval a_i */
			return s ? -x : x;
		}
		if (!alias && k >= s_aiNAL_dy[i])
		{
			x = (u + i) / 32;					  /* sample from interval i */
			return s ? -x : x;
		}
		if (ranNormalNALrest(alias, i, u, s, &x))
			return x;
	}
}
#define NALNOR_STORE 64 * 4
static unsigned int s_aiNalRan[NALNOR_STORE];
static unsigned int s_aiNalBox[NALNOR_STORE];
static int s_cNalStored = 0;

double DRanNormalNAL32Vec()
{
	unsigned int iran, u;
	int k, i, s;
	double x;

	for (;;)
	{
		if (s_cNalStored == 0)
		{
			int j, m;
			RanVecIntU(s_aiNalRan, NALNOR_STORE / 4);
			for (j = m = 0; j < NALNOR_STORE; j += 4, ++m)
			{
				iran = s_aiNalRan[m];
				s_aiNalBox[j + 0] = iran;
				s_aiNalBox[j + 1] = iran >> 8;
				s_aiNalBox[j + 2] = iran >> 16;
				s_aiNalBox[j + 3] = iran >> 24;
			}
			RanVecIntU(s_aiNalRan, NALNOR_STORE);
			s_cNalStored = j;
		}
		--s_cNalStored;
		u = s_aiNalRan[s_cNalStored];
		iran = s_aiNalBox[s_cNalStored];
		
		s = iran & 0x80;	  /* sign is most significant bit of first byte */
		i = iran & 0x7F;				  /* i in 0..127 is next seven bits */
		k = u & 0x3F;/* K in 0..63, take bits of u that are commonly unused */
		
		if (k >= s_aiNAL_q[i])		   /* do alias method (but not on tail) */
		{
			if (i != 19)          
			{	
				x = ((unsigned int)((s_aiNAL_a[i] << 25) | (u >> 7))) * M_RAN_INVM30;
				return s ? -x : x;
			}
		}
		else if (k >= s_aiNAL_dy[i])
		{
			x = ((unsigned int)((i << 25) | (u >> 7))) * M_RAN_INVM30;
			return s ? -x : x;
		}
		/*use new u, because 6 bytes were taken from original u to compute k*/
		if (ranNormalNALrest(k >= s_aiNAL_q[i], i, DRanU(), s, &x))
			return x;
	}
}

void  RanNormalSetSeedNALOrig(int *piSeed, int cSeed)
{
	RanSetSeed(piSeed, cSeed);
}
double  DRanQuanNormalNALOrig(void)
{
	return DProbNormal(DRanNormalNALOrig());
}
void  RanNormalSetSeedNAL(int *piSeed, int cSeed)
{
	RanSetSeed(piSeed, cSeed);
}
double  DRanQuanNormalNAL(void)
{
	return DProbNormal(DRanNormalNAL());
}
void  RanNormalSetSeedNAL32Vec(int *piSeed, int cSeed)
{
	RanSetSeed(piSeed, cSeed);
	s_cNalStored = 0;
}
double  DRanQuanNormalNAL32Vec(void)
{
	return DProbNormal(DRanNormalNAL32Vec());
}
/*------------------------------------ END NAL -----------------------------*/
