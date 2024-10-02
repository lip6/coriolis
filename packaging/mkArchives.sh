
    version="2.5.5"
 docGithash="b12c8613c"
     obsDir="../coriolis-obs/home:jpc-lip6/coriolis-eda"
 
  doSources="false"
     doDocs="false"
     doVEnv="false"
   doCommit="false"
 badAgument=""
 while [ $# -gt 0 ]; do
   case $1 in
     --sources) doSources="true";;
     --docs)    doDocs="true";;
     --venv)    doVEnv="true";;
     --commit)  doCommit="true";;
     --run)     doSources="true"
                doDocs="true"
                doVEnv="true"
                doCommit="true";;
     *)         badArgument="$1";;
   esac
   shift
 done
 if [ ! -z "${badArgument}" ]; then
   echo "[ERROR] patchenv.sh: Unknown argument \"${badArgument}\"."
   exit 1
 fi

 echo "Running mkArchives.sh"
 if [ "${doSources}" = "true" ]; then
   echo "* Making source file archive from Git HEAD ..."
   ./packaging/git-archive-all.sh -v --prefix coriolis-2.5.5/ --format tar.gz coriolis-${version}.tar.gz
   #git archive --prefix=coriolis-2.5.5/ --format=tar.gz -o coriolis-${version}.tar.gz HEAD
 fi
 
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
 fi

 echo "* Update files in OBS project directory."
 echo "  OBS package directory: \"${obsDir}\"."
 for distribFile in packaging/coriolis-eda.spec       \
                    packaging/patchvenv.sh            \
                    packaging/find_files.sh           \
                    packaging/coloquinte-clamp.patch  \
                    coriolis-${version}.tar.gz        \
                    venv-al9-${version}.tar.gz        \
                    coriolis-docs-${docGithash}.tar.gz; do
   if [ ! -f "${distribFile}" ]; then continue; fi
   if [[ "${distribFile}" == packaging* ]]; then
     echo "  - copy ${distribFile}."
     cp ${distribFile} ${obsDir}
   else
     echo "  - move ${distribFile}."
     mv ${distribFile} ${obsDir}
   fi
 done
 
 if [ "${doCommit}" = "true" ]; then
   pushd ${obsDir}
   osc commit
   popd
 fi

