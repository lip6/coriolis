
all: sxlib.lef
	lef2oa -lib sxlib -lef sxlib.lef

sxlib.lef:
	grep -v "END LIBRARY" cmos.lef.bak > sxlib.lef ; for i in $$(cat cells) ; do sxlib2lef $$i ; cat $$i.lef >> sxlib.lef ; done
	echo "END LIBRARY" >> sxlib.lef
	cp sxlib.lef sxlib.lef.bak

mrproper: clean
	rm -rf sxlib cds.lib

clean:
	rm -rf *.log *.slog *~ encounter* *.lef
