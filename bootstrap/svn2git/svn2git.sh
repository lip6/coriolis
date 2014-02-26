#!/bin/sh

 svn-all-fast-export --resume-from  0                \
                     --rules        ./coriolis.rules \
                     --identity-map ./authors.txt    \
                     /dsk/l1/jpc/svn2git/svn         > svn-all-fast-export.log
