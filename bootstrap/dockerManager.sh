#!/bin/bash


       showHelp=0
      showError=0
  doBuildSystem=0
        doBuild=0
          doRun=0
        doClean=0


 if [ ! -f "./docker-conf.sh" ]; then
   echo "[ERROR] Missing \"./docker-conf.sh\"."
   echo "        (wd:\"`pwd`\")"
   exit 1
 fi
 . "./docker-conf.sh"
 dockerImages="${systemImage},${coriolisImage}"


 while [ $# -gt 0 ]; do
   case $1 in
     --help)   showHelp=1;;
     --system) doBuildSystem=1;;
     --build)  doBuild=1;;
     --run)    doRun=1;;
     --clean)  doClean=1;;
     -*) NB=2; CH=`echo $1 | cut -c$NB`
         while [ "$CH" != "" ]; do
           case $CH in
             h) showHelp=1;;
             S) doBuildSystem=1;;
             b) doBuild=1;;
             r) doRun=1;;
             C) doClean=1;;
             *) showError=1; badOption="$1";;
           esac
           NB=`expr $NB + 1`
           CH=`echo $1 | cut -c$NB`
         done;;
     *) showError=1; badOption="$1";;
   esac
   shift
 done

 if [ ${showError} -ne 0 ]; then
   echo "[ERROR] Unknown argument \"${badOption}\"."
   exit 1
 fi

 if [ ${showHelp} -ne 0 ]; then
   echo "Usage: ./manager.sh [options]"
   echo "Options:"
   echo "  * [-h|--help]:   Print this help."
   echo "  * [-S|--system]: Rebuild the whole OS image."
   echo "  * [-b|--build]:  Rebuild the Coriolis image. It will remove the previous"
   echo "                   images (${dockerImages})."
   echo "  * [-r|--run]:    Recompile Alliance, Coriolis & perform benchs."
   echo "  * [-C|--clean]:  Remove container(s) & image(s)."
   exit 0
 fi 


 if [ ${doBuild} -ne 0 ] || [ ${doBuildSytem} -ne 0 ]; then
   doClean=1
 fi


 if [ ${doClean} -ne 0 ]; then
   echo "Removing \"${coriolisImage}\" docker container."
   docker rm  ${coriolisImage}
   docker rmi ${coriolisImage}

   if [ ${doBuildSystem} -ne 0 ]; then
     echo "Removing \"${systemImage}\" docker image."
     docker rm  ${systemImage}
     docker rmi ${systemImage}
   fi
 fi
 

 if [ ${doBuild} -ne 0 ]; then
   echo "Synching Alliance & Coriolis builder scripts."
   cp ../../socInstaller.py      ./root
   cp ../../allianceInstaller.sh ./root

   if [ ${doBuildSystem} -ne 0 ]; then
     echo "Build \"${systemImage}\" docker image."
     docker build -f Dockerfile.system -t ${systemImage} .
   fi

   echo "Build \"${coriolisImage}\" docker image."
   docker build -f Dockerfile.coriolis -t ${coriolisImage} .
 fi


 if [ ${doRun} -ne 0 ]; then
   docker run --name ${coriolisImage} ${coriolisImage} 
 fi
