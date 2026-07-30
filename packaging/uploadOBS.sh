
 packageName="coriolis-eda"
     version="2.5.5"
  docGithash="4e1ac7bf3"
   obsCI_CNT="248"
    obsB_CNT="1"

 rpmSources=""
 rpmSources="${rpmSources} packaging/coriolis-eda.spec"
 rpmSources="${rpmSources} packaging/coriolis-eda-rpmlintrc"
 rpmSources="${rpmSources} packaging/patchvenv.sh"
 rpmSources="${rpmSources} packaging/find_files.sh"
 rpmSources="${rpmSources} packaging/coloquinte-clamp.patch"
 rpmSources="${rpmSources} packaging/coloquinte-no-eigen3.patch"
 rpmSources="${rpmSources} packaging/lemon-no-soversion.patch"
 rpmSources="${rpmSources} coriolis-eda-${version}.tar.gz"
 rpmSources="${rpmSources} venv-al9-${version}.tar.gz"
 rpmSources="${rpmSources} coriolis-docs-${docGithash}.tar.gz"

 debSources=""
 debSources="${debSources} packaging/coriolis-eda.dsc"
 debSources="${debSources} packaging/debian.copyright"
 debSources="${debSources} packaging/debian.changelog"
 debSources="${debSources} packaging/debian.control"
 debSources="${debSources} packaging/debian.rules"
 debSources="${debSources} packaging/debian.coriolis-eda.install"
 debSources="${debSources} packaging/debian.coriolis-eda-dev.install"
 debSources="${debSources} packaging/debian.coriolis-eda-doc.install"


 echo "Running uploadOBS.sh"

 source ./packaging/uploadUtils.sh

     doDocs="false"
     doVEnv="false"

 checkRundir $0
 parseArguments $*
 simpleBuildArchive
 
 if [ "${doDocs}" = "true" ]; then
   echo "* Making snapshot archive of the documentation..."
   srcDir="`pwd`"
   pushd ../../release/install/share/doc
   tar -zcf ${srcDir}/coriolis-docs-${docGithash}.tar.gz coriolis
   popd
 fi

 if [ "${doVEnv}" = "true" ]; then
   echo "* Making virtual environment archive from <cwd>/.venv ..."
   tar --exclude=*.pyc -zcf venv-al9-${version}.tar.gz .venv .pdm-plugins
 else
   referenceVEnvSnapshot="../coriolis-obs/home:jpc-lip6/coriolis-eda/${venvSnapshot}"
   if [ ! -f "${referenceVEnvSnapshot}" ]; then
     echo "[ERROR] Venv snapshot reference not found in <${referenceVEnvSnapshot}>."
     echo "        You must checkout the coriolis-eda project *or*, if it is already there,"
     echo "        actually make the snapshot from it."
     exit 1
   fi
   cp ${referenceVEnvSnapshot} .
 fi

 copyFiles
 buildObs
 buildLocalRpm
