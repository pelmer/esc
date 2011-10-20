Start-up script
===============

::

  tar -zxf /storage/software/innocente/Bertinoro2010.tgz
  cd Bertinoro2010/
  source install.sh
  echo $CXX
  echo $CXXFLAGS
  echo $OPTFLAGS
  which c++
  $CXX $CXXFLAGS examples/paranoia.c; ./a.out
  icpc examples/paranoia.c ; ./a.out 
