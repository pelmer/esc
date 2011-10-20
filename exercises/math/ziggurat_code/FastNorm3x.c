/*---------------------------- original file below -------------------------*
 * FastNorm3.c and FastNorm3.h can be found at ftp.cs.monash.edu.au/pub/csw/
 * Please check there for the latest version.
 *
 * Change:
 */
#include <stdio.h>

/*---------------------------- original file below -------------------------*/
#include "FastNorm3.c"
/*---------------------------- original file above -------------------------*/

#include "FastNorm3.h"
static int s_iQual = 2;
void RanNormalSetWallace(int iQual)
{
	s_iQual = iQual;
}
void  RanNormalSetSeedWallace(int *piSeed, int cSeed)
{
	if (cSeed == 0)
	{
		initnorm (77, s_iQual);
	}
	else if (cSeed > 1)
	{
		initnorm (piSeed[0], piSeed[1]);
	}
}
double  DRanNormalWallace(void)
{
	return (double)FastNorm;
}
extern double  DProbNormal(double x);

double  DRanQuanNormalWallace(void)
{
	return DProbNormal(DRanNormalWallace());
}

