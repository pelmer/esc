#include <stdio.h>
#include <stdlib.h>

#include "unif01.h"
#include "bbattery.h"

#include "zigrandom.h"
#include "zignor.h"
#include "FastNorm3x.h"
#include "rnorrexpx.h"
#include "rnor_mwc8222.h"
#include "rnal.h"


int main (void) 
{
	unif01_Gen *gen;

//	RanSetSeed_MWC8222(NULL, 0);
//	gen = unif01_CreateExternGen01 ("MWC8222", DRan_MWC8222);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);
//
	RanSetRan("MWC8222");
	RanSetSeed(NULL, 0);
	gen = unif01_CreateExternGen01 ("FRanQuanNormal", FRanQuanNormal);
	bbattery_SmallCrush (gen);
	unif01_DeleteExternGen01 (gen);

//	RanNormalSetSeedRNOR(NULL, 0);
//	gen = unif01_CreateExternGen01 ("RNOR_SHR3", DRanQuanNormalRNOR);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);

//	RanSetRan("MWC8222");
//	RanNormalSetSeedRNOR_MWC8222(NULL, 0);
//	gen = unif01_CreateExternGen01 ("RNOR_MWC8222", DRanQuanNormalRNOR_MWC8222);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);

//	RanSetRan("MWC8222");
//	RanNormalSetSeedZig(NULL, 0);
//	gen = unif01_CreateExternGen01 ("ZIGNOR", DRanQuanNormalZig);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);
//
//	RanSetRan("MWC8222");
//	RanNormalSetSeedZig32Vec(NULL, 0);
//	gen = unif01_CreateExternGen01 ("VZIGNOR", DRanQuanNormalZig32Vec);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);
//
//	RanNormalSetWallace(1);
//	RanNormalSetSeedWallace(NULL, 0);
//	gen = unif01_CreateExternGen01 ("FN3(1)", DRanQuanNormalWallace);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);
//
//	RanNormalSetWallace(2);
//	RanNormalSetSeedWallace(NULL, 0);
//	gen = unif01_CreateExternGen01 ("FN3(2)", DRanQuanNormalWallace);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);
//
//	RanNormalSetSeedNALOrig(NULL, 0);
//	gen = unif01_CreateExternGen01 ("NAL (original)", DRanQuanNormalNALOrig);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);
//
//	RanNormalSetSeedNAL32Vec(NULL, 0);
//	gen = unif01_CreateExternGen01 ("VNAL", DRanQuanNormalNAL32Vec);
//	bbattery_SmallCrush (gen);
//	unif01_DeleteExternGen01 (gen);

	return 0;
}
