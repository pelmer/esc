Floating point computation: accuracy, optimization, vectorization
=================================================================

The exercises are included in the
`slides <https://agenda.infn.it/getFile.py/access?contribId=5&sessionId=0&resId=0&materialId=slides&confId=6686>`_.

The exercises for this section are in a separate git respository::

     git clone https://github.com/VinInn/FPOptimization.git
     cd FPOptimization/exercises

Then proceed as shown on the slides::

     which c++-482
     c++-482 -v
     cd FPOptimization/exercises make afloat
     ./afloat_O2
     ./afloat_fast

You should also try::

     ldd ./afloat_O2

and check that you see::
 
     libstdc++.so.6 => /opt/gcc482/lib64/libstdc++.so.6


