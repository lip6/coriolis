
#
# uploadUtils.sh provides the shared/helpers functions upon which
# uploadOBS is built.
#

     obsDir="../coriolis-obs/home:jpc-lip6/${packageName}"
     rpmDir="../coriolis-rpm"
    githash=`git log -1 --pretty=format:%h`

   buildRpm="false"
   buildObs="true"
  doSources="false"
     doDocs="N/A"
     doVEnv="N/A"
   doCommit="false"

 repoName="${packageName}"
 if [ "${packageName}" = "coriolis-eda" ]; then
   repoName="coriolis"
 fi


 printHelp () {
   echo ""
   echo "  Usage: uploadOBSs.sh [--sources] [--venv] [--commit] [--all]"
   echo ""
   echo "  Options:"
   echo "    [--rpm]     : Direct build of a AL9 rpm on the local machine."
   echo "    [--sources] : Build an archive from the HEAD of the current branch."
   echo "    [--venv]    : Copy the venv snapshot from coriolis-eda OBS local checkout."
   echo "                    <./coriolis-obs/home:jpc-lip6/coriolis-eda/${venvSnapshot}>"
   if [ "${doDocs}" != "N/A" ]; then
   echo "    [--docs]    : Take a snapshot of the current *installed* documentation."
   echo "                  Will be used for distributions unable to run doxygen+ghostscript"
   echo "                  (currently openSUSE LEAP)."
   fi
   echo "    [--commit]  : Push the files (commit) on the remote builder repository."
   echo "                  This will effectively triggers the rebuild of the packages."
   echo "                  OBS local repository is hardwired to:"
   echo "                      \"${obsDir}\""
   echo "    [--all]     : Perform all actions at once."
   echo ""
 }


 setConfig () {
   configName="$1"
   if [ "${!configName}" = "N/A" ]; then return; fi
   eval ${configName}="$2"
 }


 checkRundir ()
 {
   runDir=`pwd`
   parent=`basename ${runDir}`
   if [ "${parent}" != "${repoName}" ]; then
     echo "[ERROR] uploadOBS.sh must be run from the root directory of the package."
     echo "        That is, for example:"
     echo "           ego@home:${packageName}> ./packaging/uploadOBS --rpm"
     echo ""
     echo "        Not from \"${runDir}\"."
     exit 1
   fi
 }


 parseArguments () {
   if [ $# -eq 0 ]; then printHelp; exit 0; fi

   badAgument=""
   while [ $# -gt 0 ]; do
     case $1 in
       --help)    printHelp; exit 0;;
       --rpm)     setConfig "buildRpm"  "true"
                  setConfig "buildObs"  "false"
                  setConfig "doSources" "true"
                  setConfig "doDocs"    "true"
                  setConfig "doVEnv"    "true"
                  obsDir="${rpmDir}/SOURCES";;
       --sources) setConfig "doSources" "true";;
       --commit)  setConfig "doCommit"  "true";;
       --all)     setConfig "doSources" "true"
                  setConfig "doDocs"    "true"
                  setConfig "doVEnv"    "true"
		  setConfig "doCommit"  "true";;
       *)         badArgument="$1";;
     esac
     shift
   done
   if [ ! -z "${badArgument}" ]; then
     echo "[ERROR] uploadOBS.sh: Unknown argument \"${badArgument}\"."
     exit 1
   fi

   echo "  buildRpm=\"${buildRpm}\""
   echo "  buildObs=\"${buildObs}\""
   echo " doSources=\"${doSources}\""
   echo "    doDocs=\"${doDocs}\""
   echo "    doVEnv=\"${doVEnv}\""
   echo "  doCommit=\"${doVEnv}\""
 }


 runDoVEnv ()
 {
   if [ "${doVEnv}" = "true" ]; then
     if [ -f "./${venvSnapshot}" ]; then
       echo "* Venv snaphot already copied."
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
   fi
 }


 simpleBuildArchive ()
 {
   echo "* Using HEAD githash as release: ${githash}."
   if [ "${doSources}" = "true" ]; then
     echo "* Making source file archive from Git HEAD ..."
     ./packaging/git-archive-all.sh -v --prefix ${packageName}-${version}/ \
                                       --format tar.gz                     \
                                       ${packageName}-${version}.tar.gz
   fi
 }


 copyFiles ()
 {
   buildName="RPM (local)"
   sources="${rpmSources}"
   if [ "${buildObs}" = "true" ]; then
     buildName="OBS"
     sources="${sources} ${debSources}"
   fi

   echo "* Update files in ${buildName} project directory."
   echo "  OBS/RPM package directory: \"${obsDir}\"."
   for distribFile in ${sources} ; do
     if [ ! -f "${distribFile}" ]; then continue; fi
     if [[ "${distribFile}" == packaging* ]]; then
       echo "  - copy ${distribFile}."
       cp ${distribFile} ${obsDir}
     else
       echo "  - move ${distribFile}."
       mv ${distribFile} ${obsDir}
     fi
   done
 }


 buildObs () {
   if [ "${buildObs}" != "true" ]; then return; fi

   sed -i "s,^Release: .*,Release:        <CI_CNT>.<B_CNT>.${githash}," ${obsDir}/{packageName}.spec
   if [ "${doDocs}" != "N/A" ]; then
     sed -i "s,^%define docGithash .*,%define docGithash ${docGithash},"  ${obsDir}/${packageName}.spec
   fi
   if [ "${doCommit}" = "true" ]; then
     pushd ${obsDir}
     osc add *
     osc commit
     popd
   fi
 }


 buildLocalRpm () {
   if [ "${buildRpm}" != "true" ]; then return; fi

   rpmMacros="${rpmDir}/rpmmacros"
    specFile="${obsDir}/${packageName}.spec"

   sed -i "s,^Release: .*,Release:        ${obsCI_CNT}.${obsB_CNT}.${githash},"   ${specFile}
   if [ "${doDocs}" != "N/A" ]; then
     sed -i "s,^%define docGithash .*,%define docGithash ${docGithash},"  ${obsDir}/${packageName}.spec
   fi

   rpmbuild --load ${rpmMacros} -ba --clean ${specFile}

   for distribFile in ${rpmSources} ; do
     rpmSourceFile="${obsDir}/`basename ${distribFile}`"   
     if [ ! -f "${rpmSourceFile}" ]; then continue; fi
     echo "  - remove ${rpmSourceFile}."
     rm ${rpmSourceFile}
   done
 }
