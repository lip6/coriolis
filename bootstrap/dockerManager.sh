#!/bin/bash


         showHelp=0
        showError=0
    doBuildSystem=0
  doBuildCoriolis=0
      doBuildBash=0
          doBuild=0
            doRun=0
         doRemove=0


 if [ ! -f "./docker-conf.sh" ]; then
   echo "[ERROR] Missing \"./docker-conf.sh\"."
   echo "        (wd:\"`pwd`\")"
   exit 1
 fi
 . "./docker-conf.sh"
 dockerImages="${systemImage},${coriolisImage},${bashImage}"


 while [ $# -gt 0 ]; do
   case $1 in
     --help)            showHelp=1;;
     --build-system)    doBuildSystem=1;;
     --build-coriolis)  doBuildCoriolis=1;;
     --build-bash)      doBuildBash=1;;
     --run)             doRun=1;;
     --remove)          doRemove=1;;
     -*) NB=2; CH=`echo $1 | cut -c$NB`
         while [ "$CH" != "" ]; do
           case $CH in
             h) showHelp=1;;
             s) doBuildSystem=1;;
             c) doBuildCoriolis=1;;
             b) doBuildBash=1;;
             r) doRun=1;;
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
   echo "  * [-h|--help]:           Print this help."
   echo "  * [-s|--build-system]:   Rebuild the whole OS image."
   echo "  * [-c|--build-coriolis]: Rebuild the Coriolis image. It will remove the previous"
   echo "                           images (${dockerImages})."
   echo "  * [-b|--build-bash]:     Rebuild the Bash (shell) image. It will remove the previous"
   echo "                           image (${bashImage})."
   echo "  * [-r|--run]:            Recompile Alliance, Coriolis & perform benchs."
   echo "  * [--remove]:            Remove container(s) & image(s)."
   exit 0
 fi 


 if [ ${doBuildSystem} -ne 0 ]; then
   doBuildBash=1
   doBuildCoriolis=1
   doBuild=1
   doRemove=1
 fi

 if [ ${doBuildCoriolis} -ne 0 ]; then
   doBuildBash=1
   doBuild=1
   doRemove=1
 fi

 if [ ${doBuildBash} -ne 0 ]; then
   doBuild=1
   doRemove=1
 fi


 if [ ${doRemove} -ne 0 ]; then
   if [ ${doBuildBash} -ne 0 ]; then
     echo "Removing \"${bashImage}\" docker container."
     docker rm  ${bashImage}
     docker rmi ${bashImage}
   fi

   if [ ${doBuildCoriolis} -ne 0 ]; then
     echo "Removing \"${coriolisImage}\" docker image."
     docker rm  ${coriolisImage}
     docker rmi ${coriolisImage}
   fi

   if [ ${doBuildSystem} -ne 0 ]; then
     echo "Removing \"${systemImage}\" docker image."
     docker rm  ${systemImage}
     docker rmi ${systemImage}
   fi
 fi
 

 if [ ${doBuild} -ne 0 ]; then
   echo "Synching Alliance & Coriolis builder scripts."
   cp ../../socInstaller.py      ./root
   cp ../../dot.bashrc           ./root

   if [ ${doBuildSystem} -ne 0 ]; then
     echo "Build \"${systemImage}\" docker image."
     docker build -f Dockerfile.system -t ${systemImage} .
   fi

   if [ ${doBuildCoriolis} -ne 0 ]; then
     echo "Build \"${coriolisImage}\" docker image."
     docker build -f Dockerfile.coriolis -t ${coriolisImage} .
   fi

   if [ ${doBuildCoriolis} -ne 0 ]; then
     echo "Build \"${bashImage}\" docker image."
     docker build -f Dockerfile.bash -t ${bashImage}     .
   fi
 fi


 if [ ${doRun} -ne 0 ]; then
   docker run --rm --net=host -e DISPLAY=:0 -ti --name ${bashImage} ${bashImage} 
 fi
