/*******************************************************************

NAME:    frandom

PURPOSE: This is a low quality, simple and hopefully reproducible
         random number generator. DO NOT USE THIS if you want HIGH 
		 QUALITY PSEUDO-RANDOM numbers.  It uses a simple linear 
		 congruential algorithm with poor statistical properties.. 

         The Random numbers are uniformly distributed between 
		 input low and high long values.  The comments within the 
		 program show you how to turn this into uniform deviates
		 between 0.0 and 1.0.
		 
USAGE:   long l_random()  returns the next pseudo-random number 
                        in the sequence. 

         float f_random() returns a random float between 0 and 1

         void s_l_random(long seed, long lo, long hi) set the 
		                seed  Sets the seed and lower/upper  
						bounds for the nubers.
  
REF:     I took this algorithm and the input parameters from 
         Numerical recipies (of course).
		 
HISTORY: Written by Tim Mattson, April 1, 1999.
*******************************************************************/

void s_l_random( long, long, long);

long l_random();

float f_random();
