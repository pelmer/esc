#
#
#  --param max-completely-peel-times=1  required due to a GCC ideosyncracy
#  --param vect-max-version-for-alias-checks=30 required due to GCC limitations
#  -ftree-loop-if-convert-stores   check GCC man page

CXX = c++ -std=gnu++0x -Wall -Wstrict-overflow -Wunsafe-loop-optimizations -march=native ${ADDOPT}


VECTOPT =  -ftree-vectorizer-verbose=2 -fvisibility-inlines-hidden --param vect-max-version-for-alias-checks=30  -funsafe-loop-optimizations -ftree-loop-if-convert-stores --param max-completely-peel-times=1
IPOOPT = -flto -fvisibility=hidden



INCDIR = -I../include


.PHONY : all clean


%.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -g

%_O2.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O2 -g

%_O3.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -O3 -g

%_fast.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -Ofast -g

%_vect.o : %.cc
	$(CXX) $(INCDIR) -c $< -o $@ -Ofast ${VECTOPT} -g



%_O : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O

%_O2 : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O2

%_O3 : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -O3

%_fast : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -Ofast

%_vect : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -Ofast ${VECTOPT}

%_ipo : %.cpp
	$(CXX) $(INCDIR) $< -o $@ -Ofast ${IPOOPT}  ${VECTOPT}


clean:
	rm *.o *_O2 *_O3 *_fast *_vect *_ipo
