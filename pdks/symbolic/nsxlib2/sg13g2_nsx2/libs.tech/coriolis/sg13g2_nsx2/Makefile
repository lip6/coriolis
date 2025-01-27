# OSDI dynamic loadble module compile
# we use openvaf to compile the Verilog-A model.
# For the openvaf, see: https://openvaf.semimod.de/
# 

SOURCE = ../../../../common/libs.tech/IHP-Open-PDK/ihp-sg13g2/libs.tech/ngspice/openvaf/psp103_nqs.va
OSDI = ../../../../common/libs.tech/IHP-Open-PDK/ihp-sg13g2/libs.tech/ngspice/openvaf/psp103_nqs.osdi
OSDIFILE=psp103_nqs.osdi

$(OSDIFILE): $(SOURCE)
	openvaf $(SOURCE) -o $(OSDIFILE)
