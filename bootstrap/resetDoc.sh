#!/usr/bin/bash

 docDirs=""
 docDirs="${docDirs} hurricane/doc/analog"
 docDirs="${docDirs} hurricane/doc/hurricane"
 docDirs="${docDirs} hurricane/doc/viewer"
 docDirs="${docDirs} crlcore/doc/crlcore"
 docDirs="${docDirs} katabatic/doc"
 docDirs="${docDirs} kite/doc"
 docDirs="${docDirs} oroshi/doc"
 docDirs="${docDirs} unicorn/doc/unicorn"
 docDirs="${docDirs} vlsisapd/doc"
 docDirs="${docDirs} vlsisapd/doc"

 resetDir ()
 {
     directory="$1"
     echo "Resetting \"${directory} ..."
     if [ -d "${directory}" ]; then
       rm -rf "${directory}"
       git checkout "${directory}"
     fi
 }

 for docDir in ${docDirs}; do
     resetDir "${docDir}/html"
     resetDir "${docDir}/latex"
     resetDir "${docDir}/man"
     resetDir "${docDir}/rtf"
 done
 resetDir "documentation/output/pdfs"
