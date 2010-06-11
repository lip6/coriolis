for file in `ls *.png`; do
  convert $file -pointsize 72 -colorspace RGB -compress JPEG `basename $file .png`.pdf;
done
