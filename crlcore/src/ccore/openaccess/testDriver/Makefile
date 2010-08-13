
TESTDIR= ./testOA
TECHNOFILE= /asim/chams/etc/chams/config.freePDK45.xml
M=$$(uname -m)

all: compile lefTest run

lefTest: 
	cd sxlib2lef && make

compile: 
	./compile.sh

run: compile
	./$(M)/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR) 2>&1 | tee $@.log

debug: compile
	gdb -args ./$(M)/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR)

ddd: compile
	ddd -args ./$(M)/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR)

valgrind_simple: compile
	valgrind ./$(M)/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR) 2>&1 | tee $@.log

valgrind_full: compile
	valgrind -v --leak-check=full --show-reachable=yes ./$(M)/usr/local/bin/testOAWrapper $(TECHNOFILE) $(TESTDIR) 2>&1 | tee $@.log

mrproper: clean
	rm -rf $(TESTDIR) *.log .cadence
	cd sxlib2lef && make mrproper

clean:
	rm -rf $(M)

.PHONY: clean mrproper valgrind ddd debug run compile all lefTest
