#include <stdio.h>
#include <stdlib.h>

#include "zigrandom.h"
#include "zigtimer.h"
#include "zignor.h"
#include "FastNorm3x.h"
#include "rnorrexpx.h"
#include "rnor_mwc8222.h"
#include "rnal.h"

void TimerHeader() 
{
	printf("------------------------------------------------------------------\n");
	printf("%-30s%8s %22s %10s\n", "Name", "time", "mean", "reps");

}
void Timer(char *sName, double (*DRan)(), void (*DRanSeed)(int *, int), unsigned int cM) 
{
	unsigned int i;
	double ran, mean;
	
	printf("%-29s", sName);

	(*DRanSeed)(NULL, 0);

	StartTimer();
	for (i = 0, mean = 0.0; i < cM; ++i)
	{
		ran = (*DRan)();
		mean += ran;
	}
	mean /= cM;
	StopTimer();

	printf(" %8s %#22.15g %10d\n", GetLapsedTime(), mean, cM);
}

int main(void) 
{
	int cm = 1000000000;

	TimerHeader();

	Timer("Warming up",		DRan_MWC8222, 			RanSetSeed_MWC8222, cm/10); 
	Timer("MWC8222",		DRan_MWC8222, 			RanSetSeed_MWC8222, cm);

	RanSetRan("MWC8222");
	Timer("Polar",			DRanNormalPolar, 		RanSetSeed, cm);
	Timer("ZIGNOR",			DRanNormalZig, 			RanNormalSetSeedZig, cm);
	Timer("VZIGNOR",		DRanNormalZigVec, 		RanNormalSetSeedZigVec, cm);
	Timer("IZIGNOR",		DRanNormalZig32, 		RanNormalSetSeedZig32, cm);
	Timer("VIZIGNOR",		DRanNormalZig32Vec, 	RanNormalSetSeedZig32Vec, cm);

	Timer("NAL (original)",	DRanNormalNALOrig, 		RanNormalSetSeedNALOrig, cm);
	Timer("INAL",			DRanNormalNAL, 			RanNormalSetSeedNAL, cm);
	Timer("VNAL",			DRanNormalNAL32Vec,		RanNormalSetSeedNAL32Vec, cm);
	Timer("RNOR-SHR3",		DRanNormalRNOR, 		RanNormalSetSeedRNOR, cm);
	Timer("RNOR-MWC8222",	DRanNormalRNOR_MWC8222, RanNormalSetSeedRNOR_MWC8222, cm);

	RanNormalSetWallace(2);
	Timer("FN3(2)",			DRanNormalWallace, 		RanNormalSetSeedWallace, cm);
	RanNormalSetWallace(3);
	Timer("FN3(3)",			DRanNormalWallace, 		RanNormalSetSeedWallace, cm);
	RanNormalSetWallace(4);
	Timer("FN3(4)",			DRanNormalWallace, 		RanNormalSetSeedWallace, cm);

	return 0;
}
