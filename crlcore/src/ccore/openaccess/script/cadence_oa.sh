#!/bin/sh
# 
# we assume that the directory pointed by OPENACCESS_TOP is like in other project of the lab
# to do so we should have :
#  - included symbolic link to bin data lib and license directories from cadence OA
#  - included doc and example from si2.org OA
#  - included a symbolic link to an installation of headers from si2.org OA
# 
# this script set environment variable for crlcore compilation with cadence OA
# with these hypothesis
#
#OA_VERSION=oa_v22.04.057
#ARCH=linux_rhel40_64
#

export OPENACCESS_TOP=$TEMP/CADENCE_OA

#get OPUS_TOP and tools
#from virtuoso editor
source $TEMP/IC613.sh

# we need oa + specific cadence libs ...
export OA_LIB_PATH=$OPENACCESS_TOP/lib:$OPUS_TOP/tools.lnx86/lib/64bit
echo $OA_LIB_PATH

export OA_INCLUDE_PATH=$OPENACCESS_TOP/include
echo $OA_INCLUDE_PATH

export LD_LIBRARY_PATH=$OA_LIB_PATH:$LD_LIBRARY_PATH

#for plugins .plg files
export OA_PLUGIN_PATH=$OPENACCESS_TOP/data/plugins

#get encouter router too
source $TEMP/EDI910.sh

