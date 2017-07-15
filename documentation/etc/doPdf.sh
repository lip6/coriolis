#!/bin/sh


 document=`basename -s .rst ${@:$#}`
  sources=${@:1:$(($# -1))}

#echo "Args:     $*"
 echo "Document: ${document}.rst"
 echo "Sources:  ${sources}"

 rm -f ${document}.rst
 for source in ${sources}; do
   source=`basename ${source}`
   if [ "${source}" != "pdfHeader.rst" ]; then
     tail -n +4 ${source} | \
       sed 's,:ref:`\([^`]*\)`,`\1`_,' \
       >> ${document}.rst
   else
     cat ${source} >> ${document}.rst
   fi
 done
 export TEXINPUTS=../etc/images//:./images//:
 rst2latex --use-latex-toc --stylesheet=../etc/SoC-ReST.tex ${document}.rst ${document}-raw.tex
 sed 's, \\& \\\\multicolumn{2}{l|}{,  \\& \\\\multicolumn{2}{p{0.6\\\\DUtablewidth}|}{,' ${document}-raw.tex > ${document}.tex
 pdflatex ${document}
 pdflatex ${document}
 
 rm -f ${document}.rst     \
       ${document}-raw.tex \
       ${document}.tex     \
       ${document}.log     \
       ${document}.out     \
       ${document}.aux     \
       ${document}.toc
