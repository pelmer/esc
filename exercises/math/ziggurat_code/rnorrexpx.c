/*---------------------------- original file below -------------------------*
 * rnorrexp.c can be found at www.jstatsoft.org/v05/i08
 * Please check there for the latest version.
 *
 * Changes:
 *
 * 1. use int throughout, int is 32 bits on all used computers,
 *    but long is 64 bits on 64-bit linux
 * 2. The original code uses fabs in the comparison. This is unnecessarily
 *    slow: the argument is an integer, as is the right-hand side.
 *    This applies to all use of fabs in the code, so can be redefined here.
 * 3. Use doubles instead of floats
 */
#include <math.h>						   /* include before redefinitions! */
#include <stdlib.h>										  /* needed for abs */
#define long int
#define fabs abs
#define float double

/*---------------------------- original file below -------------------------*/
#include "rnorrexp.c"
/*---------------------------- original file above -------------------------*/

void  RanNormalSetSeedRNOR(int *piSeed, int cSeed)
{
	if (cSeed == 0)
		zigset(17235321);
	else if (cSeed > 0)
		zigset(*piSeed);
}
double  DRanNormalRNOR(void)
{
	return (double)RNOR;
}
double  DProbNormal(double x);

double  DRanQuanNormalRNOR(void)
{
	return DProbNormal(DRanNormalRNOR());
}
