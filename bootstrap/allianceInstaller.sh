#!/bin/bash

 #. /etc/*-release
 #arch="Linux.el9"
 #if [ "`echo ${VERSION} | cut -c 1`" == "7" ]; then
 #  echo "Building for RHEL 7"
 #  arch="Linux.el7_64"
 #fi

  arch_str=$((uname -srm))
  if [[ ${arch_str} =~ Linux.*(el9|al9).*x86_64 ]]; then
    echo "Building for RHEL 9"
    arch="Linux.el9"
  elif [[ ${arch_str} =~ Linux.*(el7|al7).*x86_64 ]]; then
    echo "Building for RHEL 7"
    arch="Linux.el7"
  else
    echo "Building for Generic Linux"
    arch="Linux.x86_64"
  fi

  nightly=""
  if [[ "`pwd`" =~ /nightly/ ]]; then
    nightly="/nightly"
  fi
  root="${HOME}${nightly}"

  getString ()
  {
    string=`echo $1 | cut -d '=' -f 2-` 
    echo $string
  }

  while [ $# -gt 0 ]; do
    case $1 in
      --github-runner=*) echo "Using Github/runner profile.";
                         onGithub="true"
                         root=`getString $1`;;
    esac
    shift
  done

  if [ "${onGithub}" = "true" ]; then
    arch="Linux.x86_64"
  fi


      srcDir=${root}/coriolis-2.x/src/alliance/alliance/src
  commonRoot=${root}/coriolis-2.x/${arch}/Release.Shared
 #commonRoot=${root}/coriolis-2.x/${arch}/Debug.Shared
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
