#!/bin/bash

 scriptPath="${BASH_SOURCE[0]}";
 
 if [ -h "${scriptPath}" ] then
   while [ -h "${scriptPath}" ];  do scriptPath=`readlink "${scriptPath}"`; done
 fi
 
 pushd . > /dev/null
 cd `dirname ${scriptPath}` > /dev/null
 sysconfDir=`pwd`;
 popd  > /dev/null
 
 coriolisEnvPy="${sysconfDir}/coriolisEnv.py"

 if [ -e ${coriolisEnvPy} ]; then
   eval "`${coriolisEnvPy} --v2 --release --shared --python`"
 else
   echo "[ERROR] Missing ${coriolisEnvPy} script."
 fi
