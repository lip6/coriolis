#!/bin/bash

qmake -project
case "`uname -s`" in
  Linux)  qmake;;
  Darwin) qmake -spec macx-g++;;
esac
make -j2 && make clean
