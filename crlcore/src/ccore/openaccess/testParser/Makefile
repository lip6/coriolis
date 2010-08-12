
TECHNOFILE= /asim/chams/etc/chams/config.freePDK45.xml

all: compile 

compile: 
	./compile.sh

run:
	./x86_64/usr/local/bin/testOAWrapper \
	"/dsk/l1/misc/caba/OA_BENCHMARKS/NangateOpenCellLibrary_PDKv1_3_v2009_07/openaccess" \
	"NangateOpenCellLibrary" \
	"/dsk/l1/misc/caba/OA_BENCHMARKS/NangateOpenCellLibrary_PDKv1_3_v2009_07/openaccess" \
	"NCSU_FreePDK_45nm"

mrproper: clean
	rm -rf *.log .cadence

clean:
	rm -rf x86_64


.PHONY: clean mrproper compile all run
