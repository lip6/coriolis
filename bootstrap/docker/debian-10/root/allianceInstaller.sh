#!/bin/bash

      srcDir=${HOME}/coriolis-2.x/src/alliance/alliance/src
  commonRoot=${HOME}/coriolis-2.x/Linux.el7_64/Release.Shared
    buildDir=${commonRoot}/build
  installDir=${commonRoot}/install

  export    ALLIANCE_TOP=${installDir}
  export LD_LIBRARY_PATH=${installDir}/lib:${LD_LIBRARY_PATH}

  cd ${srcDir}
 # Skip doc generation to avoid pulling TeXLive in docker images.
  sed -i 's,dirs="\$newdirs documentation",dirs="$newdirs",' ./autostuff
  ./autostuff clean
  ./autostuff
  mkdir -p ${buildDir}
  cd ${buildDir}
  ${srcDir}/configure --prefix=${ALLIANCE_TOP} --enable-alc-shared
  make -j1 install
