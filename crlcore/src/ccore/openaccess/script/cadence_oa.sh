#!/bin/sh
# 
# we assume we work on linux for 32bit or 64 bits x86.
# 
# this script set environment variable for crlcore compilation with cadence OA
# first we set OPENACCESS_LIB to the oa directory containing the OA version we want to use
# second we set OPENACCESS_INCLUDE to where the OA include we want to use are
# third we set the OA_PLUGIN_PATH and systems PATHS so the plugins and shared lib will load.
#

# use oa from virtuoso
OA_VERSION=oa_v22.04.057
# get OPUS_TOP and tools
# from virtuoso editor (needed)
source ~opus/bin/NCSU-CDK16.sh


ARCH=$(uname -i)

OPENACCESS_TOP=$OPUS_TOP/$OA_VERSION

# we need oa + specific cadence libs ...
case "$ARCH" in
    "x86_64")
	export OA_LIB_DIR=$OPENACCESS_TOP/lib/linux_rhel40_64/opt
	export LD_LIBRARY_PATH=/lib64:$LD_LIBRARY_PATH:$OA_LIB_DIR:$OPUS_TOP/tools.lnx86/lib/64bit
	;;
    *)
	export OA_LIB_DIR=$OPENACCESS_TOP/lib/linux_rhel40_32/opt
	export LD_LIBRARY_PATH=/lib:$LD_LIBRARY_PATH:$OA_LIB_DIR:$OPUS_TOP/tools.lnx86/lib
	;;
esac

export PATH=$PATH:$OPENACCESS_TOP/bin:~opus/6.1.0/Linux/EDI-9.10/bin

# for plugins .plg files
export OA_PLUGIN_PATH=$OPENACCESS_TOP/data

# where the si2 hearders have been installed
# note that $TEMP is /dsk/l1/misc/$(USER) directory
export OA_INCLUDE_DIR=$TEMP/OA_HEADER
#si on utilise OA de si2 on aura
#export OA_INCLUDE_DIR=$OPENACCESS_TOP/include
