README.rst
=================

Update August 2, 2023
---------------------
	coriolis-2.x/src/alliance-check-toolkit/benchs/artel6502/cmos/README.rst

Introduction
------------
This file describes steps to execute the bench ``artel6502``  and tranforms it from RTL to layout using open source free toolchain **coriolis2**. It builds upon a python tool **Doit** with the designflow **dodo.py**.  

	| LOGICAL SYTHESIS  	= Yosys  
	| PHYSICAL SYTHESIS 	= Coriolis  
	| TECHNOLOGY 		= cmos 
	| NETLISTS 	= Arlet6502 \\ 
	|           		cpu     \\    
        |           		ALU

                                                             
	 This directory contains the files.
		  * Arlet6502.v 	-- bench file in verilog
		  * ALU.v		-- sub modules of bench
		  * cpu.v		-- sub modules of bench
		  * cpu_syncreset.v	-- sub modules of bench
		  * doDesign.py 	-- supportive modules
		  * dodo.py 		


Environment configuration for coriolis
---------------------------------------
Launch:
  bash 
   $ source eval \`{Home}/coriolis-2.x/<OS>.<DISTRIB>/Release.Shared/install/etc/coriolis2/coriolisEnv.py\`
   
	Note:  Where <OS> is the name of your operating system and <DISTRIB> your distribution.

Help
----
``doit list``	-- shows all possible commands 


Running:
---------
Run these commands to generate the physical layout of the given netlist. 

1. ``doit yosys``  	-- This command performs logical systhesis.  It takes Arlet6502.v as an input and generate Arlet6502.blif  

2. ``doit b2v``    	-- It transforms Arlet6502.blif to coriolis netlist format. it takes Arlet6502.blif as an input and generates two files, Arlet6502.vst (structrural VHDL), and Arlet6502.spi (structural spice model). 

3. ``doit pnr`` 	-- It performs placement and routing and genarates final physical layout file Arlet6502_cts_r.ap. It also generates Arlet6502_cts_r.spi and Arlet6502_cts_r.vst. It depends on Arlet6502.vst and Arlet6502.spi. You can run this command directly, it performs frist previous steps automatically.    
      
4. ``doit cgt``		-- Open physcial layout file on cgt. click on File->open cell, enter the name of the design,  such as Arlet6502_cts_r.       
  
5. ``doit druc`` 	-- Symbolic layout checker, It is a design rule check (druc). It depends on arlet6502_cts_r and generates an error file arlet6502_cts_r.drc. It also generates arlet6502_cts_r_rng.gds and arlet6502_cts_drc.gds. 

6. ``doit cougar``	-- Symbolic layout extractor, it generates arlet6502_cts_r_ext.vst.

7.  ``doit lvx``	-- Gate Netlist Comparator, it compares chip layout against extracted netlist.   


Clean
------
``doit clean_flow``	-- remove all generated files.  

