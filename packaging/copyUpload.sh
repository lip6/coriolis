#!/bin/bash

 repoDirs=""
 repoDirs="${repoDirs} alliance"
 repoDirs="${repoDirs} tas-yagle"
 repoDirs="${repoDirs} coriolis-pdk-sky130-c4m"
 repoDirs="${repoDirs} coriolis-pdk-ihpsg13g2"
 repoDirs="${repoDirs} coriolis-pdk-ihpsg13g2-c4m"
 repoDirs="${repoDirs} coriolis-pdk-gf180mcu"
 repoDirs="${repoDirs} coriolis-pdk-gf180mcu-c4m"
 repoDirs="${repoDirs} coriolis-pdk-nsx2"

 for repoDir in ${repoDirs}; do
   echo "./coriolis/packaging/uploadUtils.sh  -->  ../${repoDir}/packaging/"
   cp ./packaging/uploadUtils.sh ../${repoDir}/packaging
 done
