foreach cxxflags ( "" "-msse3" " -fassociative-math -fno-signed-zeros -fno-trapping-math -freciprocal-math -fno-math-errno -ffinite-math-only")
    echo $cxxflags
    c++ -O2  -Wall $cxxflags  fpe.cpp fpefun.cc
    foreach opt (d i r o u x q l n s) 
	./a.out $opt
    end
    foreach opt (d i r o u x q l n s) 
	./a.out $opt fpe
    end

end
