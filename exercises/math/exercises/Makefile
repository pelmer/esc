#
#  requires latest-greatest gcc (gcc 4.7.2 october release)
#  --param max-completely-peel-times=1  required due to a GCC ideosyncracy
#  --param vect-max-version-for-alias-checks=30 required due to GCC limitations
#  -ftree-loop-if-convert-stores   check GCC man page

CXX = c++ ${ADDOPT} -std=gnu++11 -Wall -Wno-format  -Wstrict-overflow -Wunsafe-loop-optimizations -march=corei7 
# -march=native


VECTOPT =  -ftree-vectorizer-verbose=2 -fvisibility-inlines-hidden --param vect-max-version-for-alias-checks=100  -funsafe-loop-optimizations -ftree-loop-if-convert-stores 
# --param max-completely-peel-times=1 -fno-tree-pre
IPOOPT = -flto -fvisibility=hidden



INCDIR = -I../include -I../vdt/include


.PHONY : run all clean afloat intLoop floatLoop look patriot sum lsum nsum tsum fcos precision foperations kernel assocMath SimpleVectorization condExpr OptimizeIt Benchmark CrossingPlane FourVec AoSvsSoA BenchSoa ApproxMath Approximations

all : afloat intLoop floatLoop look patriot fcos precision foperations kernel assocMath SimpleVectorization condExpr OptimizeIt Benchmark CrossingPlane FourVec AoSvsSoA BenchSoa lsum ApproxMath Approximations

sum : sum_O sum_O2 sum_O3 sum_fast sum_vect sum_ipo

lsum :
	rm sum_O sum_O2 sum_O3 sum_fast sum_vect sum_ipo; make sum ADDOPT="-fopenmp"
nsum :
	rm sum_O sum_O2 sum_O3 sum_fast sum_vect sum_ipo; make sum ADDOPT="-fopenmp -DNONASTY"
tsum :
	rm sum_O sum_O2 sum_O3 sum_fast sum_vect sum_ipo; make sum ADDOPT="-fopenmp -DNOPRINT"

afloat : afloat_O2 afloat_fast
intLoop : intLoop_O intLoop_O2
floatLoop : floatLoop_O2
fcos : fcos_O2
look : look_O2
patriot : patriot_O2
precision : precision_O2 precision_fast
foperations : foperations_O2 foperations_fast
kernel : kernel_O2.o kernel_fast.o
assocMath : assocMath_O2.o assocMath_fast.o assocMath_vect.o
SimpleVectorization : SimpleVectorization_O2.o SimpleVectorization_fast.o SimpleVectorization_vect.o
condExpr : condExpr_defvect.o
OptimizeIt : OptimizeIt_O2.o  OptimizeIt_fast.o  OptimizeIt_vect.o
Benchmark : Benchmark_O2  Benchmark_O3  Benchmark_fast  Benchmark_vect  Benchmark_ipo 
CrossingPlane : CrossingPlane_vect CrossingPlane_ipo
FourVec : FourVec_O2.o FourVec_O3.o FourVec_vect.o
AoSvsSoA : AoSvsSoA_O2.o AoSvsSoA_vect.o
BenchSoa : BenchSoa_O2 BenchSoa_vect
Approximations : Approximations_O2 Approximations_vect
ApproxMath : ApproxMath_O2 ApproxMath_vect

%_O.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O -g 

%_O2.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O2 -g


%_O2.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O2 -g

%_O3.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O3 -g

%_fast.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -Ofast -g

%_vect.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -Ofast ${VECTOPT} -g

%_defvect.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O3 -g -ftree-vectorizer-verbose=2



%_O : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O

%_O2 : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O2

%_O3 : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O3

%_fast : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -Ofast

%_defvect : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O3 -ftree-vectorizer-verbose=2

%_vect : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -Ofast ${VECTOPT}


%_ipo : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -Ofast ${IPOOPT}  ${VECTOPT}

run : all
	./run.sh

clean:
	rm *.o *_O *_O2 *_O3 *_fast *_vect *_ipo
