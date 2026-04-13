#!/bin/sh

 echo "Running pachenv.sh"
 VIRTUAL_ENV="`pwd`/.venv"
 if [ ! -d "${VIRTUAL_ENV}" ]; then
   echo "[ERROR] patchenv.sh: Virtual environment directory not found:"
   echo "        \"${VIRTUAL_ENV}\""
   exit 1
 fi

    useSystemPackages="False"
 removeVEnvWatchfiles="False"
            removePip="False"
          badArgument=""
 while [ $# -gt 0 ]; do
   case $1 in
     --use-system-packages)    useSystemPackages="True";;
     --remove-venv-watchfiles) removeVEnvWatchfiles="True";;
     --remove-pip)             removePip="True";;
     *)                        badArgument="$1";;
   esac
   shift
 done
 if [ ! -z "${badArgument}" ]; then
   echo "[ERROR] patchenv.sh: Unknown argument \"${badArgument}\"."
   exit 1
 fi

 PYTHON_BIN="/usr/bin/python3"
 if [ -x "/usr/bin/python3.11" ]; then PYTHON_BIN="/usr/bin/python3.11"; fi
 PYTHON_VERSION=`$PYTHON_BIN -c 'import sys; print("python{0.major}.{0.minor}".format(sys.version_info))'`
 echo "* Updating venv path to ${VIRTUAL_ENV}"
 sed -i "s,VIRTUAL_ENV=.*,VIRTUAL_ENV=\"${VIRTUAL_ENV}\","   .venv/bin/activate
 for binfile in `ls .venv/bin/*`; do
   if [ -L "${binfile}" ]; then continue; fi
   echo "* Updating python interpreter in ${binfile}"
   sed -i "s,#!.*venv/bin/python,#!${VIRTUAL_ENV}/bin/python," ${binfile}
 done

 if [ "${removeVEnvWatchfiles}" = "True" ]; then 
   rm -r ${VIRTUAL_ENV}/bin/watchfiles
   rm -r ${VIRTUAL_ENV}/lib/python3.9/site-packages/watchfiles*
 fi
 if [ "${removePip}" = "True" ]; then 
   rm -r ${VIRTUAL_ENV}/bin/pip*
   rm -r ${VIRTUAL_ENV}/lib/python3.9/site-packages/pip* 
 fi
 if [ "${useSystemPackages}" = "True" ]; then
    sed -i "s,include-system-site-packages.*=.*false,include-system-site-packages.*=.*true," .venv/pyvenv.cfg
 fi
 if [ "${PYTHON_VERSION}" != "python3.9" ]; then
   echo "* Re-linking toward ${PYTHON_VERSION}"
   mv     ${VIRTUAL_ENV}/lib/python3.9 ${VIRTUAL_ENV}/lib/${PYTHON_VERSION}
   ln -sf ${VIRTUAL_ENV}/lib/${PYTHON_VERSION} ${VIRTUAL_ENV}/lib/python3.9 
   ln -sf python3                              ${VIRTUAL_ENV}/bin/${PYTHON_VERSION}
   ln -sf /usr/bin/${PYTHON_VERSION}           ${VIRTUAL_ENV}/bin/python3
 fi
 ls -al .venv/bin
 source ${VIRTUAL_ENV}/bin/activate
 echo "Check Python interpreter links:"
 for interpreter in ${VIRTUAL_ENV}/bin/python    \
                    ${VIRTUAL_ENV}/bin/python3   \
                    ${VIRTUAL_ENV}/bin/python3.9 \
                    ${VIRTUAL_ENV}/bin/${PYTHON_VERSION}; do
   echo "* ${interpreter} is `${interpreter} -c 'import sys; print("python{0.major}.{0.minor}".format(sys.version_info))'`"
 done
 deactivate
 ls -al /usr/bin/python*

 if [ -f .venv/bin/pdm     ]; then touch .venv/bin/pdm;     fi
 if [ -f .venv/bin/meson   ]; then touch .venv/bin/meson;   fi
 if [ -f .venv/bin/pelican ]; then touch .venv/bin/pelican; fi

 exit 0
