#!/bin/bash

 document=`basename -s .rst $1`
 current_source_dir=$2
 stylesheet=$3
 rst2latex=$4
 pdflatex=$5
 echo "Generating: ${document}.pdf from $current_source_dir with stylesheet $stylesheet"

 $rst2latex --use-latex-toc --stylesheet=$stylesheet ${document}.rst ${document}-raw.tex
 sed "s%\(include.*\){\(\..*\)}%\1{$current_source_dir/\2}%" \
     's, \\& \\\\multicolumn{2}{l|}{,  \\& \\\\multicolumn{2}{p{0.6\\\\DUtablewidth}|}{,' \
     ${document}-raw.tex > ${document}.tex
 $pdflatex ${document}

 rm -f ${document}.rst     \
       ${document}-raw.tex \
       ${document}.tex     \
       ${document}.log     \
       ${document}.out     \
       ${document}.aux     \
       ${document}.toc
