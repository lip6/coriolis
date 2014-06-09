#!/bin/sh

 confFiles=""
#confFiles="${confFiles} open.png"
#confFiles="${confFiles} closed.png"
#confFiles="${confFiles} jquery.js"
#confFiles="${confFiles} dynsections.js"
 confFiles="${confFiles} SoC.css"
#confFiles="${confFiles} header.html"

       rootDir="${HOME}/coriolis-2.x/src/coriolis"
 sourceToolDoc="${rootDir}/hurricane/doc/hurricane"
      toolDocs=""
      toolDocs="${toolDocs} ${rootDir}/hurricane/doc/viewer"
      toolDocs="${toolDocs} ${rootDir}/crlcore/doc/crlcore"
      toolDocs="${toolDocs} ${rootDir}/katabatic/doc"
      toolDocs="${toolDocs} ${rootDir}/kite/doc"
      toolDocs="${toolDocs} ${rootDir}/unicorn/doc/unicorn"

 for toolDoc in $toolDocs; do
   echo "- $toolDoc"
   if [ -d "$toolDoc" ]; then
     for file in $confFiles; do
       cp "${sourceToolDoc}/${file}" "${toolDoc}/${file}"
     done
   else
     echo "[ERROR] Tool <${toolDoc}> doesn't exist."
   fi
 done
 echo "Do not forget to edit CMakeLists.txt, customSummary.html, header.html"
 echo "& doxyfile then create a \\mainpage in the documentation."
 echo "   DISABLE_INDEX = NO"
 exit 0
