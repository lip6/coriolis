#!/bin/bash

 fossEDADir="/dsk/l1/distributions/fossEDA/al/9_devel"
     rpmDir="/dsk/l1/jpc/coriolis-2.x/src/coriolis-rpm"

 repoDirs=""
 repoDirs="${repoDirs} alliance"
#repoDirs="${repoDirs} tas-yagle"
#repoDirs="${repoDirs} coriolis"   # Associated package is "coriolis-eda".
#repoDirs="${repoDirs} coriolis-pdk-sky130-c4m"
#repoDirs="${repoDirs} coriolis-pdk-ihpsg13g2"
#repoDirs="${repoDirs} coriolis-pdk-ihpsg13g2-c4m"
#repoDirs="${repoDirs} coriolis-pdk-gf180mcu"
#repoDirs="${repoDirs} coriolis-pdk-gf180mcu-c4m"
#repoDirs="${repoDirs} coriolis-pdk-nsx2"

 for repoDir in ${repoDirs}; do
   echo "Rebuilding devel packages for ${repoDir}"
   (cd ../${repoDir}; ./packaging/uploadOBS.sh --rpm)
 done

 rm ${rpmDir}/RPMS/x86_64/*debug* 

 rpmsign --addsign ${rpmDir}/SRPMS/*.rpm       \
	           ${rpmDir}/RPMS/x86_64/*.rpm \
                   ${rpmDir}/RPMS/noarch/*.rpm

 mv ${rpmDir}/SRPMS/*.rpm       ${fossEDADir}/Source/src/
 mv ${rpmDir}/RPMS/x86_64/*.rpm ${fossEDADir}/x86_64/x86_64/
 mv ${rpmDir}/RPMS/noarch/*.rpm ${fossEDADir}/x86_64/noarch/

 for rpmRepoDir in ${fossEDADir}/source/src \
                   ${fossEDADir}/x86_64 ; do
   (cd ${rpmRepoDir}; createrepo_c --update -d .; dnf repomanage --keep 2 .)
 done
