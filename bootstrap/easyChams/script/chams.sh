#!/bin/sh
VERSION="0.5"
DATE="25/01/2010"
version() {
  echo "$O version $VERSION - $DATE - by D.Dupuis"
  echo ""
}

usage() {
  echo "Usage $0 -s sourceRD -i installRD -b buildRD -t tool [-druc]"
  echo ""
  echo "  Mandatory arguments:"
  echo "    -p path  : path to chams root directory"
  echo "    -t tool  : specify which tool to compile (there may be several -t arguments"
  echo "    -m mode  : specify build mode (Release or Debug)"
  echo ""
  echo "  Optionnal arguments:"
  echo "    -s : enable static build"
  echo "    -d : generate documentation (if available)"
  echo "    -u : run svn update before compilation"
  echo "    -c : clean build before compilation"
  echo ""
  exit 1
}

resume() {
    echo ""
    echo "Source directory  : $SOURCE"
    echo "Install directory : $INSTALL"
    echo "Builb directory   : $BUILD"
    echo "Tools             : $TOOLS"
    echo "Generate doc      : $DOC"
    echo "Svn update        : $SVNUP"
    echo ""
}

guessOs ()
{
  case "`uname -srm`" in
    Linux*el5*x86_64)     OSTYPE="Linux.SLSoC5x_64";;
    Linux*el5*)           OSTYPE="Linux.SLSoC5x";;
    Linux*2.6.23.13*SoC*) OSTYPE="Linux.SLSoC5x";;
    Linux*EL*x86_64*)     OSTYPE="Linux.SLA4x_64";;
    Linux*SLA*)           OSTYPE="Linux.SLA4x";;
    Linux*EL*i686*)       OSTYPE="Linux.SLA4x";;
    Linux*FC2*)           OSTYPE="Linux.FC2";;
    Linux*i686*)          OSTYPE="Linux.i686";;
    SunOS\ 5*)            OSTYPE="Solaris";;
    Darwin*)              OSTYPE="Darwin";;
    *)                    OSTYPE="`uname -sr`";;
  esac
}


prepareEnv() {
  guessOs
  if [ ! -d "$SOURCE" ]
  then
    echo "Specified root path does not exist"
    exit 1
  fi
  ROOT="$SOURCE/$OSTYPE/$MODE.$LIBMODE"
  INSTALL="$ROOT/install"
  BUILD="$ROOT/build"
  export HURRICANE_TOP="$INSTALL"
  export IO_TOP="$INSTALL"
  export CHAMS_TOP="$INSTALL"
  export DYLD_LIBRARY_PATH="$INSTALL/lib"
}

compile() {
  prepareEnv
  for TOOL in $TOOLS
  do
    if [ $SVNUP -eq 1 ]
    then
      cd "$SOURCE/$TOOL"
      if [ `svn status -u | grep "^M.*\*" | wc -l` -gt 0 ]
      then
        echo "THERE MIGHT BE CONFLICTS WHILE SVN UPDATING"
        echo " -> Please update manually"
        exit 1
      fi
      #svn update --accept postpone # par defaut on prendra la decision en cas de conflits plus tard (au cas ou le status n'est pas suffisant)
      svn update # par defaut on prendra la decision en cas de conflits plus tard (au cas ou le status n'est pas suffisant)
    fi
    if [ ! -d "$BUILD/$TOOL" ]
    then
      echo "$BUILD/$TOOL does not exist: create it now !"
      mkdir -p "$BUILD/$TOOL"
    fi
    cd "$BUILD/$TOOL"
    if [ $? -ne 0 ]
    then
      echo "Cannot find build directory"
      exit 1
    fi
    if [ $CLEAN -eq 1 ]
    then
      make clean
      rm -rf CMakeCache.txt CMakeFiles Makefile cmake_install.cmake install_manifest.txt
    fi
    echo "#### Now compiling $TOOL ####"
    echo ""
    cmake -D "CMAKE_BUILD_TYPE:STRING=$MODE" -D "BUILD_DOC:STRING=$DOC" -D "BUILD_SHARED_LIBS:STRING=$SHARED" $SOURCE/$TOOL
    make "DESTDIR=$INSTALL" -j8 install
    if [ $? -ne 0 ]
    then
      exit 1
    fi
    echo ""
  done
}

SOURCE=""
INSTALL=""
BUILD=""
TOOLS=""
OSTYPE=""
MODE=""
LIBMODE="Shared"
SHARED="ON"
DOC="OFF"
SVNUP=0
CLEAN=0

if [ $# -lt 1 ]; then
  usage
  exit 1
fi

while getopts :p:t:m:sduc option
do
  case "${option}"
  in
    p) SOURCE=${OPTARG};;
    t) TOOL=${OPTARG}
       TOOLS="$TOOLS $TOOL";;
    m) MODE=${OPTARG};;
    s) SHARED="OFF"
       LIBMODE="Static";;
    d) DOC="ON";;
    u) SVNUP=1;;
    c) CLEAN=1;;
    *) version
       usage;;
  esac
done
shift "$(( $OPTIND - 1 ))"

#resume
version
compile
