#!/bin/bash

 printHelp () {
   echo ""
   echo "  Usage: doPdf.sh [--buildDir=<buildDir>]"
   echo "                  [--sourceDir=<sourceDir>]"
   echo "                  [--stylesheet=<stylesheet>]"
   echo "                  [--rst2latex=<rst2latex_bin>]"
   echo "                  [--pdflatex=<pdflatex_bin>]"
   echo "                  <document>"
   echo ""
   echo "  Mandatory argument:"
   echo "    <document> : There must be one, and exactly one reStructuredText document to"
   echo "                 be processed. With the \".rst\" extension."
   echo ""
   echo "  Options:"
   echo "    [--buildDir=]   : The directory where the pdfs will be created."
   echo "                      Default: the current working directory."
   echo "    [--sourceDir=]  : The top directory where the documentation source is located."
   echo "                      Default: the current working directory."
   echo "    [--stylesheet=] : The LaTeX style to be used."
   echo "                      Default: no style at all."
   echo "    [--rst2latex=]  : The absolute path to the rst2latex binary."
   echo "                      Default: rst2latex (let the OS find out)."
   echo "    [--pdflatex=]   : The absolute path to the pdflatex binary."
   echo "                      Default: pdflatex (let the OS find out)."
   echo ""

 }

 if [ $# -eq 0 ]; then printHelp; exit 0; fi


 getString ()
 {
   string=`echo $1 | cut -d '=' -f 2-` 
   echo $string
 }


   document=""
   buildDir="`pwd`"
  sourceDir="`pwd`"
 stylesheet="None"
  rst2latex="rst2latex"
   pdflatex="pdflatex"
 badAgument=""
 while [ $# -gt 0 ]; do
   case $1 in
     --buildDir=*)   buildDir=`getString $1`;;
     --sourceDir=*)  sourceDir=`getString $1`;;
     --stylesheet=*) stylesheet=`getString $1`;;
     --rst2latex=*)  rst2latex=`getString $1`;;
     --pdflatex=*)   pdflatex=`getString $1`;;
     -*)             badArgument="$1";;
     *)              if [ ! -z "${document}" ]; then
                       printHelp
                       echo "[ERROR] doPdf.sh: You must supply only one document."
                       echo "        (\"${document}\" and \"$1\")."
                       exit 1
                     fi
                     sourceDocumentDir=`dirname $1`
                     document=`basename -s .rst $1`;;
   esac
   if [ ! -z "${badArgument}" ]; then
     echo "[ERROR] doPdf.sh: Unknown argument \"${badArgument}\"."
     printHelp
     exit 1
   fi
   shift
 done
 if [ -z "${document}" ]; then
   printHelp
   exit 0
 fi

 echo "doPdf.sh:"
 echo "  sourceDir:         ${sourceDir}"
 echo "  sourceDocumentDir: ${sourceDocumentDir}"
 echo "  document:          ${document}"
 echo "  stylesheet:        ${stylesheet}"
 echo "  buildDir:          ${buildDir}"
 echo "  rst2latex:         ${rst2latex}"
 echo "  pdflatex:          ${pdflatex}"

 export TEXINPUTS="${sourceDir}/${sourceDocumentDir}:${sourceDir}//:${TEXINPUTS}"
 echo "TEXINPUTS=${TEXINPUTS}"
 cd ${sourceDir}/${sourceDocumentDir}
 ${rst2latex} --use-latex-toc            \
 	      --stylesheet=${stylesheet} \
 	      ${document}.rst            \
 	      ${buildDir}/${document}.tex
 cd ${buildDir}
 sed -i 's,\(include.*\){\(\.\./\)*\(.*\)},\1{\3},' ${document}.tex
 sed -i 's,\(include.*\){\(\./images\)\+\(.*\)},\1{images\3},' ${document}.tex
 sed -i 's, \\& \\\\multicolumn{2}{l|}{,  \\& \\\\multicolumn{2}{p{0.6\\\\DUtablewidth}|}{,' \
        ${document}.tex
 ${pdflatex} ${document}
 ${pdflatex} ${document}

 rm -f ${document}.tex     \
       ${document}.log     \
       ${document}.out     \
       ${document}.aux     \
       ${document}.toc     \
       texput.log
