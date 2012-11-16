#!/bin/bash

#scriptPath="$BASH_ARGV[0]"
 scriptPath="/soc/coriolis2/etc/coriolis2/coriolis2.sh"
 
 if [ -h "${scriptPath}" ]; then
   while [ -h "${scriptPath}" ];  do scriptPath=`readlink "${scriptPath}"`; done
 fi
 
 pushd . > /dev/null
 cd `dirname ${scriptPath}` > /dev/null
 sysconfDir=`pwd`;
 popd  > /dev/null
 
 coriolisEnvPy="${sysconfDir}/coriolisEnv.py"
 echo $coriolisEnvPy

 if [ -e ${coriolisEnvPy} ]; then
   eval "`${coriolisEnvPy} --release --shared`"
 else
   echo "[ERROR] Missing ${coriolisEnvPy} script."
 fi

 unset scriptPath sysconfDir coriolisEnvPy
