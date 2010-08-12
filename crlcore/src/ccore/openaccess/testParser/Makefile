
TECHNOFILE= /asim/chams/etc/chams/config.freePDK45.xml

all: compile 

compile: 
	./compile.sh

run:
	./x86_64/usr/local/bin/testOAWrapper

mrproper: clean
	rm -rf *.log .cadence

clean:
	rm -rf x86_64


.PHONY: clean mrproper compile all run
