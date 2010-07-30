
all:
	./compile.sh

run:
	./x86_64/usr/local/bin/testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml /tmp/testOA 2>&1 | tee err.log

debug:
	gdb -args ./x86_64/usr/local/bin/testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml /tmp/testOA

ddd:
	ddd -args ./x86_64/usr/local/bin/testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml /tmp/testOA

rmtmp:
	rm -rf /tmp/*

clean:
	rm -rf x86_64
