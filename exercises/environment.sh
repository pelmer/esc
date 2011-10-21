export VO_CMS_SW_DIR=/storage/software/tuura/sw
export SCRAM_ARCH=slc5_amd64_gcc434
. $VO_CMS_SW_DIR/cmsset_default.sh
eval $(cd /storage/software/tuura/sw/CMSSW_3_9_3 && scram runtime -sh)
export CMS_PATH=$VO_CMS_SW_DIR
