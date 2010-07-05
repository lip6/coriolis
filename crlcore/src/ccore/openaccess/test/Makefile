

all:
	./compile.sh

run:
	cd x86_64/usr/local/bin && ./testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml

debug:
	cd x86_64/usr/local/bin && gdb -args ./testOAWrapper /asim/chams/etc/chams/config.freePDK45.xml

clean:
	rm -rf x86_64
