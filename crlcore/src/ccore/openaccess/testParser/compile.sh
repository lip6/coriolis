#!/bin/bash
ARCH=`uname -m`

if [ ! -e "./compile.sh" ]; then
  echo "You must run compile.sh in its own direcotry : ./compile.sh"
  exit 1
fi

if [ ! -d "$ARCH/build" ]; then
  echo "Creating build directory"
  mkdir -p $ARCH/build
fi

cd $ARCH/build && cmake ../.. && make DESTDIR=.. -j2 install
