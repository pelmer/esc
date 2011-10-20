export CXX=g++-451
export CXXFLAGS="-O2 -std=gnu++0x -msse4"
export OPTFLAGS="-fassociative-math -freciprocal-math -fno-math-errno -fno-signed-zeros -fno-trapping-math -ffinite-math-only"
export CXXFLAGS="${CXXFLAGS} -ftree-vectorize -ftree-vectorizer-verbose=1 -pthread -fPIC -fopenmp"
export LD_LIBRARY_PATH=${PWD}/lib:/opt/gcc451/lib64/:${LD_LIBRARY_PATH}
alias g++="g++-451"
alias c++="g++-451"

