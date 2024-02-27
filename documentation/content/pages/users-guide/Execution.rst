.. -*- Mode: rst -*-

Executing a Benchmark
=====================
Alliance-check-toolkit comprises of several benchmarks and examples. To execute a circuit with coriolis, first, you have to clone alliance-check-toolkit. 

.. code-block:: sh
   
   dummy@lepka:src> cd ~/coriolis-2.x/src
   dummy@lepka:src> git clone https://github.com/lip6/alliance-check-toolkit 

Before, executing a circuit, install doit and yosys. 

To install python doit.

.. code-block:: sh

   dummy@lepka:~> pip install doit==0.33.0
   	

To install yosys on |Debian| 12 or |Ubuntu| 22 or  compatible distributions

Prerequisites of yosys, 

.. code-block:: sh

   dummy@lepka:~> sudo apt-get install tcl-dev readline-dev libffi-dev
   dummy@lepka:~> sudo apt-get update
   dummy@lepka:~> sudo apt-get install yosys
	
	
To install yosys on |RHEL| 9  or compatible distributions

.. code-block:: sh

   dummy@lepka:~> sudo yum install clang tcl-devel readline-devel bison flex libffi-devel
   dummy@lepka:~> git clone https://github.com/YosysHQ/yosys.git
   dummy@lepka:~> cd yosys
   dummy@lepka:yosys> make clean
   dummy@lepka:yosys> sudo make install

After installting doit and yosys, navigate to exacte directory of benchmark, as an example, arlet6502

.. code-block:: sh	

   dummy@lepka:~> coriolis-2.x/src/alliance-check-toolkit/benchs/arlet6502/sky130_c4m  
   dummy@lepka:sky130_c4m> ../../../bin/crlenv.py doit list

you can perform all the steps using the follwoing commands, such as,  for sythesis,

.. code-block:: sh	

  dummy@lepka:sky130_c4m> ../../../bin/crlenv.py doit yosys


Help
----
``doit list``	-- shows all possible commands 



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
