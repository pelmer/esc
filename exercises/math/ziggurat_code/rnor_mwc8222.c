#include <math.h>						   /* include before redefinitions! */
#include <stdlib.h>										  /* needed for abs */

#include "zigrandom.h"

#define MWC_R  256
#define MWC_A  LIT_UINT64(809430660)
#define MWC_AI 809430660
#define MWC_C  362436
static unsigned int s_uiStateMWC = MWC_R - 1;
static unsigned int s_uiCarryMWC = MWC_C;
static unsigned int s_auiStateMWC[MWC_R];
static UINT64 t;

#define MWC8222 \
	(s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1),		   \
	t = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC,	   \
	s_uiCarryMWC = (unsigned int)(t >> 32),					   \
	s_auiStateMWC[s_uiStateMWC] = (unsigned int)t)

#define UNI (.5 + (signed) MWC8222*.2328306e-9)
#define IUNI MWC8222

static int hz;
static unsigned int iz, kn[128];
static double wn[128],fn[128];

#define RNOR_MWC8222 (hz=MWC8222, iz=hz&127, (abs(hz)<kn[iz])? hz*wn[iz] : nfix_MWC8222())

/* nfix() generates variates from the residue when rejection in RNOR occurs. */

double nfix_MWC8222(void)
{
const double r = 3.442620f;     /* The start of the right tail */
static double x, y;
 for(;;)
  {  x=hz*wn[iz];      /* iz==0, handles the base strip */
     if(iz==0)
       { do{ x=-log(UNI)*0.2904764; y=-log(UNI);}	/* .2904764 is 1/r */
        while(y+y<x*x);
        return (hz>0)? r+x : -r-x;
       }
                         /* iz>0, handle the wedges of other strips */
      if( fn[iz]+UNI*(fn[iz-1]-fn[iz]) < exp(-.5*x*x) ) return x;

     /* initiate, try to exit for(;;) for loop*/
      hz=MWC8222;
      iz=hz&127;
      if(abs(hz)<kn[iz]) return (hz*wn[iz]);
  }

}

/*--------This procedure sets the seed and creates the tables------*/

static void zigset_MWC8222()
{  const double m1 = 2147483648.0;
   double dn=3.442619855899,tn=dn,vn=9.91256303526217e-3, q;
   int i;

/* Set up tables for RNOR */
   q=vn/exp(-.5*dn*dn);
   kn[0]=(dn/q)*m1;
   kn[1]=0;

   wn[0]=q/m1;
   wn[127]=dn/m1;

   fn[0]=1.;
   fn[127]=exp(-.5*dn*dn);

    for(i=126;i>=1;i--)
    {dn=sqrt(-2.*log(vn/dn+exp(-.5*dn*dn)));
     kn[i+1]=(dn/tn)*m1;
     tn=dn;
     fn[i]=exp(-.5*dn*dn);
     wn[i]=dn/m1;
    }
}

void  RanNormalSetSeedRNOR_MWC8222(int *piSeed, int cSeed)
{
	zigset_MWC8222();
	GetInitialSeeds(s_auiStateMWC, MWC_R, piSeed && cSeed ? piSeed[0] : 0, 0);
}
double  DRanNormalRNOR_MWC8222(void)
{
	return (double)RNOR_MWC8222;
}
double  DRanQuanNormalRNOR_MWC8222(void)
{
	return DProbNormal(DRanNormalRNOR_MWC8222());
}
