#!/bin/bash

  srcDir=${HOME}
 gitHash="6edca05"

 cd ${srcDir}
 git clone https://github.com/cliffordwolf/yosys.git
 cd yosys
 git checkout ${gitHash}
 make config-gcc
 make GIT_REV=${gitHash} -j4
 make GIT_REV=${gitHash} install
