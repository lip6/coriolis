#!/bin/bash

if [ ! -e "./compile.sh" ]; then
  echo "You must run compile.sh in its own direcotry : ./compile.sh"
  exit 1
fi

if [ ! -d "build" ]; then
  echo "Creating build directory"
  mkdir build
fi

cd build && cmake .. && make DESTDIR=.. -j2 install
