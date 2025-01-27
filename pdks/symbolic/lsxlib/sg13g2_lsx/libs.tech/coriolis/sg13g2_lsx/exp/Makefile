## PDK sg13g2_lsx
# Building .rds from .exp files

# Hard coded path... Oh brother ...
#TODO: fix this ASAP e.g. using env variable to some meaningful place
exp_bin = $$HOME/coriolis-2.x/release/install/bin/exp

../sg13g2_lsx.rds: \
		sg13g2_lsx.main.exp \
		sg13g2_lsx.rules.exp \
		sg13g2_lsx.values.exp \
		symbolic_lsx.rules.exp \
		lambda_lsx.exp
	$(exp_bin) -v2 -o $@ $<
