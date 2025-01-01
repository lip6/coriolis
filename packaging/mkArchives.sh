
    version="2.5.5"
 docGithash="17783c54f"
     obsDir="../coriolis-obs/home:jpc-lip6/coriolis-eda"

 printHelp () {
   echo ""
   echo "  Usage: mkArchives.sh [--sources] [--docs] [--venv] [--commit] [--run]"
   echo ""
   echo "  Options:"
   echo "    [--sources] : Build an archive from the HEAD of the current branch."
   echo "    [--docs]    : Take a snapshot of the current *installed* documentation."
   echo "                  Will be used for distributions unable to run doxygen+ghostscript"
   echo "                  (currently openSUSE LEAP)."
   echo "    [--venv]    : Take a snapshot of the virtual environment."
   echo "    [--commit]  : Push the files (commit) on the remote builder repository."
   echo "                  This will effectively triggers the rebuild of the packages."
   echo "                  OBS local repository is hardwired to:"
   echo "                      \"${obsDir}\""
   echo "    [--run]     : Perform all actions at once."
   echo ""

 }

 if [ $# -eq 0 ]; then printHelp; fi

    githash=`git log -1 --pretty=format:%h`
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
 echo "* Using HEAD githash as release: ${githash}."
 if [ "${doSources}" = "true" ]; then
   echo "* Making source file archive from Git HEAD ..."
   ./packaging/git-archive-all.sh -v --prefix coriolis-eda-2.5.5/ --format tar.gz coriolis-eda-${version}.tar.gz
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
 for distribFile in packaging/coriolis-eda.spec               \
                    packaging/patchvenv.sh                    \
                    packaging/find_files.sh                   \
                    packaging/coloquinte-clamp.patch          \
                    coriolis-eda-${version}.tar.gz            \
                    venv-al9-${version}.tar.gz                \
                    coriolis-docs-${docGithash}.tar.gz        \
		    packaging/coriolis-eda.dsc                \
		    packaging/debian.copyright                \
		    packaging/debian.changelog                \
		    packaging/debian.control                  \
		    packaging/debian.rules                    \
		    packaging/debian.coriolis-eda.install     \
		    packaging/debian.coriolis-eda-dev.install \
		    packaging/debian.coriolis-eda-doc.install \
		    ; do
   if [ ! -f "${distribFile}" ]; then continue; fi
   if [[ "${distribFile}" == packaging* ]]; then
     echo "  - copy ${distribFile}."
     cp ${distribFile} ${obsDir}
   else
     echo "  - move ${distribFile}."
     mv ${distribFile} ${obsDir}
   fi
 done
 
 sed -i "s,^Release: *1,Release:        <CI_CNT>.<B_CNT>.${githash}," ${obsDir}/coriolis-eda.spec
 sed -i "s,^%define docGithash .*,%define docGithash ${docGithash},"  ${obsDir}/coriolis-eda.spec
 if [ "${doCommit}" = "true" ]; then
   pushd ${obsDir}
   osc commit
   popd
 fi

