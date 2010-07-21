

all:
	./compile.sh

run: 
	cd x86_64/usr/local/bin && ./testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml /tmp/testOA

debug: 
	cd x86_64/usr/local/bin && gdb -args ./testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml /tmp/testOA

ddd: 
	cd x86_64/usr/local/bin && ddd -args ./testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml /tmp/testOA

rmtmp:
	rm -rf /tmp/*

clean:
	rm -rf x86_64
