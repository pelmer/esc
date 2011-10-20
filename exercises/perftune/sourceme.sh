. gcc.sh

#export INTEL=/opt/intel
#export PATH=/opt/gcc-4.5.1/bin/:$PATH
#export LD_LIBRARY_PATH=/opt/gcc-4.5.1/lib64:$LD_LIBRARY_PATH

#source /opt/intel/Compiler/11.1/072/bin/iccvars.sh intel64
 
#source /opt/intel/tbb/2.0/em64t/cc4.1.0_libc2.4_kernel2.6.16.21/bin/tbbvars.sh
#source /afs/cern.ch/sw/IntelSoftware/linux/setup.sh
#source /afs/cern.ch/sw/IntelSoftware/linux/x86_64/tbb/2.0/em64t/cc4.1.0_libc2.4_kernel2.6.16.21/bin/tbbvars.sh
#source /afs/cern.ch/sw/IntelSoftware/linux/x86_64/Compiler/11.1/072/bin/iccvars.sh intel64
. /opt/intel/Compiler/11.1/072/bin/iccvars.sh  intel64

icc --version
echo "The environment has been set up"
