Setting up igprof locally
=========================

In order to set a web-visible area for your igprof reports, first read the
description of how to produce the sqlite/web report itself and the generic
description of how to set up the web area in::

  http://igprof.sourceforge.net/analysis.html

The specific information you need to set the web area up Bertinoro at ESC is
described here.

Each student ``studNN`` has a web-visible area which should be visible on the
machine you are using via the following path::

  /storage/nfs_esc10/studNN/

Within that area you should be able to do::

  mkdir /storage/nfs_esc10/studNN/public_html
  mkdir /storage/nfs_esc10/studNN/public_html/cgi-bin

Then you should be able to set up the igprof-specific part as::

  cp `which igprof-navigator`
  /storage/nfs_esc10/studNN/public_html/cgi-bin/igprof-navigator.py

(the extra ".py" is needed due to the local webserver configuration) and then
you can copy in the igprof sqlite report file you have produced, for example::

  mkdir /storage/nfs_esc10/studNN/public_html/cgi-bin/data
  cp $WHEREEVER/igtest_perf.sql3 /storage/nfs_esc10/studNN/public_html/cgi-bin/data

All should then be ready and you should be able to view the report with::

  http://137.204.203.59/~studNN/cgi-bin/igprof-navigator.py/igtest_perf/

You should see something analogous to these examples (G4 simulation from CMS)::

  # perf report
  http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias02_perf/

  # heap snapshot (after 50 events)
  http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03.50_live/

  # total dynamic memory allocations
  http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03_total/

If you are adventurous and would like to build igprof standalone yourself,
outside of the provided (pre-built) software environment, try the following
recipe::

  # Set up a few envvars to simplify the build recipe
  export INSTAREA=/x/y/z   # Wherever you want to install the software

  # Get the source tarballs for libatomic_ops, libunwind and igprof
  wget http://www.hpl.hp.com/research/linux/atomic_ops/download/libatomic_ops-7.2alpha4.tar.gz
  wget -O libunwind.tar.gz "http://git.savannah.gnu.org/gitweb/?p=libunwind.git;a=snapshot;h=982d590ddb778f0d301fe2e5647abd9135a2f9bc;sf=tgz"
  wget -O igprof-5.7.0.tar.gz "http://sourceforge.net/projects/igprof/files/igprof-5.7.0.tar.gz/download"

  # Get two patches for libunwind
  wget -O libunwind-mincore.patch "http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/COMP/CMSDIST/libunwind-mincore.patch?revision=1.1"
  wget -O libunwind-trace.patch "http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/COMP/CMSDIST/libunwind-trace.patch?revision=1.1"

  # Get a recent version of cmake
  wget http://www.cmake.org/files/v2.8/cmake-2.8.1.tar.gz

  # Build libatomic
  gtar xzvf libatomic_ops-7.2alpha4.tar.gz
  cd libatomic_ops-7.2alpha4
  ./configure --prefix=$INSTAREA
  make install
  cd ../

  # Build libunwind
  gtar xzf libunwind.tar.gz
  cd libunwind-982d590
  patch -p1 < ../libunwind-mincore.patch
  patch -p1 < ../libunwind-trace.patch
  autoreconf -i
  ./configure CPPFLAGS="-I$INSTAREA/include" --prefix=$INSTAREA --disable-block-signals
  make install
  cd ../

  # Build cmake
  gtar xzf cmake-2.8.1.tar.gz
  cd cmake-2.8.1
  mkdir -p $INSTAREA/tmpcmake
  ./configure --prefix=$INSTAREA/tmpcmake
  make
  make install
  cd ../
  export PATH=$INSTAREA/tmpcmake/bin:$PATH

  # Build igprof
  gtar xzf igprof-5.7.0.tar.gz
  cd igprof-5.7.0
  cmake -DCMAKE_INSTALL_PREFIX=$INSTAREA .
  make
  make install

  # Clean up the temporary cmake build
  rm -fr $INSTAREA/tmpcmake
