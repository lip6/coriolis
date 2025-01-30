-------------------------------------------
Process to make an realTechno.rds file
------------------------------------------
/users/cao/mariem/coriolis/2023symbolic/StudySky130/README.rst

version January 16, 2023, MML
        February 2, 2023, MML
        February 26, 2023, MML
        March 1, 2023, MML

techno target : sky130 (SkyWater130)

library target : sxlib
/users/cao/mariem/coriolis-2.x/src/alliance-check-toolkit/cells/sxlib

symbolic rules : 1 lambda is used (as poly gate minimal width (i.e. gate length of transistor (L)))

------------------------------
techno target : SkyWater130 nm
https://skywater-pdk.readthedocs.io/en/main

process:
https://skywater-pdk.readthedocs.io/en/main/rules/assumptions.html#process-stack-diagram

layers:
https://skywater-pdk.readthedocs.io/en/main/rules/layers.html#gds-layers-information

rules:
https://skywater-pdk.readthedocs.io/en/main/rules/periphery.html#x

in LIP6:
/users/soft/freepdks/src/skywater-pdk/docs/rules/device-details/cap_mim
/users/soft/freepdks/src/skywater-pdk/libraries/sky130_fd_pr/latest
/users/cao/mariem/coriolis-2.x/src/alliance-check-toolkit/pdkmaster/C4M.Sky130/libs.tech/coriolis/techno/etc/coriolis2/node130/sky130

stream in/out map file
https://skywater-pdk.readthedocs.io/en/main/rules/layers.html#gds-layers-information

GDS index:
https://skywater-pdk.readthedocs.io/en/main/rules/layers.html#gds-layers-information

library target :  sxlib
/users/cao/mariem/coriolis-2.x/src/alliance-check-toolkit/cells/sxlib


------------------------------

Programm used is : exp
    It is part of alliance
    The alliance environment has to be launched. This can be done using:
    source sourceme

Files requested :
sky130d1_1.main.exp
   computing the data (scaling and extensions) for each layer, based on lambda value, and creation of the rds file
   including : symbolic_1.rules.exp
               sky130d1_1.rules.exp (real DRM rules, see below), 
               sky130d1_1.values.exp (GDS mask index see below)

sky130d1_1.nice.exp 
   blot few layers and make the layout more beautifull !

sky130d1_1.rules.exp 
   real rules extracted manually from the PDK DRM, unit is micron

sky130d1_1.values.exp 
   electrical values for the extractor, layer cif names and layer gds numbers

lambda_1.exp 
   lambda computation, based on the investigation of all rules, and keeping the worse  value (larger value)

sourceme
   to set alliance environment
   source sourceme

Makefile
   make to generate sky130d1_1.rds file
   uses files described above
   
--------------------
2024 Jul 31
This file is now obsolete and will be rewritten soon.
