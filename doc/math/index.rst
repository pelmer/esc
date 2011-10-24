Floating point computation: accuracy, optimization, vectorization
=================================================================

Browse `exercise material <../exercises/math/>`_.  The exercises are included in the
`slides <http://agenda.infn.it/getFile.py/access?contribId=5&sessionId=0&resId=0&materialId=slides&confId=3839>`_.

For environment setup, you can either do as shown on slides::

  tar -zxf /storage/software/innocente/FPOptimization2011.tgz
  cd FPOptimization2011
  source setup.sh

Or use the git area on the student server::

  cd esc/exercises/math
  . ./setup.sh

Then proceed as shown on the slides::

  which c++-470 
  c++-470 -v 

  cd exercises 
  make afloat 
  ./afloat_O2 
  ./afloat_fast
