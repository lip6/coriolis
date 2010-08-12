
TESTDIR= ./testOA
TECHNOFILE= /asim/chams/etc/chams/config.freePDK45.xml

all: compile lefTest run

lefTest: 
	cd sxlib2lef && make

compile: 
	./compile.sh

run: compile
	./x86_64/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR) 2>&1 | tee $@.log

debug: compile
	gdb -args ./x86_64/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR)

ddd: compile
	ddd -args ./x86_64/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR)

valgrind_simple: compile
	valgrind ./x86_64/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR) 2>&1 | tee $@.log

valgrind_full: compile
	valgrind -v --leak-check=full --show-reachable=yes ./x86_64/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR) 2>&1 | tee $@.log

mrproper: clean
	rm -rf $(TESTDIR) *.log .cadence
	cd sxlib2lef && make mrproper

clean:
	rm -rf x86_64


.PHONY: clean mrproper valgrind ddd debug run compile all lefTest
