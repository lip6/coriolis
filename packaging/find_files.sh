#!/bin/sh


 getString ()
 {
   string=`echo $1 | cut -d '=' -f 2-` 
   echo $string
 }
 

            mode="main"
       buildroot=""
 python3Sitearch=""
     badArgument=""
 while [ $# -gt 0 ]; do
   case $1 in
     --buildroot=*)        buildroot="`getString $1`";;
     --python3-sitearch=*) python3Sitearch="`getString $1`";;
     --main)               mode="main";;
     --devel)              mode="devel";;
     --docs)               mode="docs";;
     *)                    badArgument="$1";;
   esac
   shift
 done
 if [ ! -z "${badArgument}" ]; then
   echo "[ERROR] find_files.sh: Unknown argument \"${badArgument}\"."
   exit 1
 fi
 if [ -z "${buildroot}" ]; then
   echo "[ERROR] find_files.sh: --buildroot argument \"${badArgument}\"."
   exit 1
 fi
 if [ ! -d "${buildroot}" ]; then
   echo "[ERROR] find_files.sh:  buildroot directory not found:"
   echo "        \"${buildroot}\"."
   exit 1
 fi

 if [ "${mode}" = "main" ]; then
   rm -f coriolis-files.main
   find "${buildroot}/usr/bin"   -maxdepth 1 -mindepth 1  >> coriolis-files.main 
   find "${buildroot}/usr/lib64" -maxdepth 1 -name '*.so' >> coriolis-files.main 
   if [ -d "${buildroot}/${python3Sitearch}/coriolis" ]; then
     find "${buildroot}/${python3Sitearch}/coriolis" -type d -exec /bin/echo "%dir {}" \; >> coriolis-files.main 
     find "${buildroot}/${python3Sitearch}/coriolis" -type f >> coriolis-files.main 
   fi
   sed -i "s,${buildroot},," coriolis-files.main 
 fi

 if [ "${mode}" = "devel" ]; then
   rm -f coriolis-files.devel
   find "${buildroot}/usr/include" -maxdepth 1 -name '*.hpp' >> coriolis-files.devel 
   sed -i "s,${buildroot},," coriolis-files.devel 
 fi

 if [ "${mode}" = "docs" ]; then
   rm -f coriolis-files.docs
   if [ -d "${buildroot}/usr/share/doc/coriolis" ]; then
     find "${buildroot}/usr/share/doc/coriolis" -type d -exec /bin/echo "%dir {}" \; >> coriolis-files.docs
     find "${buildroot}/usr/share/doc/coriolis" -type f >> coriolis-files.docs
   fi
   sed -i "s,${buildroot},," coriolis-files.docs
 fi

 exit 0
