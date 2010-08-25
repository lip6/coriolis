#!/bin/sh

 if [ -z "$BOOTSTRAP_TOP" ]; then
   export BOOTSTRAP_TOP=/asim/coriolis2
   echo "BOOTSTRAP_TOP is not in the environment. Using default:"
   echo "  $BOOTSTRAP_TOP"
 fi

 if [ -z "$CORIOLIS_TOP" ]; then
   export CORIOLIS_TOP=/asim/coriolis2
   echo "CORIOLIS_TOP is not in the environment. Using default:"
   echo "  $CORIOLIS_TOP"
 fi

 if [ -z "$MANGROVE_TOP" ]; then
   export MANGROVE_TOP=/asim/mangrove
   echo "MANGROVE_TOP is not in the environment. Using default:"
   echo "  $MANGROVE_TOP"
 fi

 rootDir=${HOME}/mangrove

 procs=1
 if [ $# -gt 0 ]; then procs="$1"; fi

 echo "Using $procs threads."

 ${rootDir}/src/bootstrap/buildMangrove.py \
     --root=${rootDir}       \
     --project=mangrove      \
     --project=caiman        \
     --make="-j${procs} install"
