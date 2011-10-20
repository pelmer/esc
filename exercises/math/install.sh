source setBert.sh
export LD_LIBRARY_PATH=${PWD}/lib:${LD_LIBRARY_PATH}
cd Minuit2-5.24.00/
./configure --prefix=$PWD/../
make clean
make -j 12
make install
cd ../
$CXX -O2 -std=gnu++0x -msse4 -ftree-vectorize -ftree-vectorizer-verbose=1 -pthread -fPIC -fopenmp -lMinuit2 solutions/loglike.cpp -Iinclude -Llib
time ./a.out k

