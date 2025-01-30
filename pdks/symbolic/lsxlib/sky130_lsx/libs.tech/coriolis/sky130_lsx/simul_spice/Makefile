# see README.md in the same directory

.SILENT:

    ACT_TOP = ~/coriolis-2.x/src/alliance-check-toolkit#
 TECHNO_SPI = $(ACT_TOP)/pdkmaster/C4M.Sky130/libs.tech/ngspice/C4M.Sky130_logic_lib.spice

C ?= inv_x4 inv_x4_chain a2_x2 sff1_x4 sram2bw2r2

help :
	echo ""
	echo "Usage:"
	echo "    make all [C=<Target>]: Simulate typic, fast, slow for Target"
	echo "    make [tt|ss|ff] [C=<Target>]: Simulate only one mode for Target"
	echo "    make clean: Delete all files except the sources"
	echo ""
	echo "    Examples"
	echo "        make all"
	echo "        make tt C=sff1_x4"
	echo "        make tt C=\"sff1_x4 a2_x2\""
	echo ""
	echo "    Default Targets:"
	echo "        $(C)"
	echo ""

tt:
	for c in $(C); do\
		make CELL=$$c $$c/$$c.tt ;\
	done

ss:
	for c in $(C); do\
		make CELL=$$c $$c/$$c.ss ;\
	done

ff:
	for c in $(C); do\
		make CELL=$$c $$c/$$c.ff ;\
	done

all: tt ss ff

define NGSPICE 
	cd $(CELL)
	cpp -D_TECHNO=$(TECHNO_SPI) -D_TEMP=$1 -D_MODEL=$2 -D_VDD=$3 -D_VMAX=$4 $*.cir |\
	grep -v "^#" > $@.cir
	-ngspice -o $@.log -b $@.cir
	echo "set title '$@ T=$1 M=$2 V=$3'" >> $@.plt
	echo "set term png" >> $@.plt
	echo "set term png" >> $@.plt
	echo "set output \""$@".png\"" >> $@.plt
	echo "replot" >> $@.plt
	echo Generated files : $@.log, $@.png, $@.plt, $@.data
	cd ..
	echo ""
endef

%.ff:%.cir ; $(call NGSPICE,0,ff,1.85,1.884)
%.tt:%.cir ; $(call NGSPICE,25,tt,1.8,1.799)
%.ss:%.cir ; $(call NGSPICE,70,ss,1.75,1.774)

c clean: 
	-killall gnuplot_x11 
	-rm -f */*.*.*
