#!/bin/sh

 cd images
 for file in *.fig; do
    echo "Translating $file into PDF..."
    fig2dev -L pdf -p dummy $file `basename $file .fig`.pdf
 done
 cd ..

 exit 0
