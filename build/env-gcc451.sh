export CMS_PATH=/storage/software/main/current/sw.cms
export SCRAM_ARCH=slc5_amd64_gcc451 VO_CMS_SW_DIR=$CMS_PATH
. $VO_CMS_SW_DIR/cmsset_default.sh
eval $(cd $VO_CMS_SW_DIR/$SCRAM_ARCH/cms/cmssw/CMSSW_4_4_1 && scram runtime -sh)

_gcc_dir=$(dirname $(which gcc))
function c++-32() { $_gcc_dir/c++ -m32 -Wa,--32 ${1+"$@"}; }
function g++-32() { $_gcc_dir/g++ -m32 -Wa,--32 ${1+"$@"}; }
function gcc-32() { $_gcc_dir/gcc -m32 -Wa,--32 ${1+"$@"}; }
