
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler 1999.10
-- FILENAME     :    sxlib_FTGS.vhd
-- FILE CONTENTS:    Entity, Structural Architecture(FTGS),
--                   and Configuration
-- DATE CREATED :    Mon May  7 10:19:50 2001
-- 
-- LIBRARY      :    sxlib
-- DATE ENTERED :    Thu Dec 21 11:24:55 MET 2000
-- REVISION     :    1.200000
-- TECHNOLOGY   :    cmos
-- TIME SCALE   :    1 ns
-- LOGIC SYSTEM :    IEEE-1164
-- NOTES        :    FTGS, Timing_mesg(TRUE), Timing_xgen(FALSE), GLITCH_HANDLE
-- HISTORY      :
-- 
----------------------------------------------------------------

----- CELL a2_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity a2_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.261 ns;
      tpdi0_q_F                      :	Time := 0.388 ns;
      tpdi1_q_R                      :	Time := 0.203 ns;
      tpdi1_q_F                      :	Time := 0.434 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end a2_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of a2_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_a2_x2_FTGS of a2_x2 is
   for FTGS
   end for;
end CFG_a2_x2_FTGS;


----- CELL a2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity a2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.338 ns;
      tpdi0_q_F                      :	Time := 0.476 ns;
      tpdi1_q_R                      :	Time := 0.269 ns;
      tpdi1_q_F                      :	Time := 0.518 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end a2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of a2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_a2_x4_FTGS of a2_x4 is
   for FTGS
   end for;
end CFG_a2_x4_FTGS;


----- CELL a3_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity a3_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.395 ns;
      tpdi0_q_F                      :	Time := 0.435 ns;
      tpdi1_q_R                      :	Time := 0.353 ns;
      tpdi1_q_F                      :	Time := 0.479 ns;
      tpdi2_q_R                      :	Time := 0.290 ns;
      tpdi2_q_F                      :	Time := 0.521 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end a3_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of a3_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "00000001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_a3_x2_FTGS of a3_x2 is
   for FTGS
   end for;
end CFG_a3_x2_FTGS;


----- CELL a3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity a3_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.478 ns;
      tpdi0_q_F                      :	Time := 0.514 ns;
      tpdi1_q_R                      :	Time := 0.428 ns;
      tpdi1_q_F                      :	Time := 0.554 ns;
      tpdi2_q_R                      :	Time := 0.356 ns;
      tpdi2_q_F                      :	Time := 0.592 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end a3_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of a3_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "00000001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_a3_x4_FTGS of a3_x4 is
   for FTGS
   end for;
end CFG_a3_x4_FTGS;


----- CELL a4_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity a4_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.374 ns;
      tpdi0_q_F                      :	Time := 0.578 ns;
      tpdi1_q_R                      :	Time := 0.441 ns;
      tpdi1_q_F                      :	Time := 0.539 ns;
      tpdi2_q_R                      :	Time := 0.482 ns;
      tpdi2_q_F                      :	Time := 0.498 ns;
      tpdi3_q_R                      :	Time := 0.506 ns;
      tpdi3_q_F                      :	Time := 0.455 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end a4_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of a4_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0000000000000001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_a4_x2_FTGS of a4_x2 is
   for FTGS
   end for;
end CFG_a4_x2_FTGS;


----- CELL a4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity a4_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.505 ns;
      tpdi0_q_F                      :	Time := 0.650 ns;
      tpdi1_q_R                      :	Time := 0.578 ns;
      tpdi1_q_F                      :	Time := 0.614 ns;
      tpdi2_q_R                      :	Time := 0.627 ns;
      tpdi2_q_F                      :	Time := 0.576 ns;
      tpdi3_q_R                      :	Time := 0.661 ns;
      tpdi3_q_F                      :	Time := 0.538 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end a4_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of a4_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0000000000000001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_a4_x4_FTGS of a4_x4 is
   for FTGS
   end for;
end CFG_a4_x4_FTGS;


----- CELL an12_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity an12_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.200 ns;
      tpdi0_q_F                      :	Time := 0.168 ns;
      tpdi1_q_R                      :	Time := 0.285 ns;
      tpdi1_q_F                      :	Time := 0.405 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end an12_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of an12_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0100",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_an12_x1_FTGS of an12_x1 is
   for FTGS
   end for;
end CFG_an12_x1_FTGS;


----- CELL an12_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity an12_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.461 ns;
      tpdi0_q_F                      :	Time := 0.471 ns;
      tpdi1_q_R                      :	Time := 0.269 ns;
      tpdi1_q_F                      :	Time := 0.518 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end an12_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of an12_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0100",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_an12_x4_FTGS of an12_x4 is
   for FTGS
   end for;
end CFG_an12_x4_FTGS;


----- CELL ao2o22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity ao2o22_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.572 ns;
      tpdi0_q_F                      :	Time := 0.451 ns;
      tpdi1_q_R                      :	Time := 0.508 ns;
      tpdi1_q_F                      :	Time := 0.542 ns;
      tpdi2_q_R                      :	Time := 0.432 ns;
      tpdi2_q_F                      :	Time := 0.627 ns;
      tpdi3_q_R                      :	Time := 0.488 ns;
      tpdi3_q_F                      :	Time := 0.526 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end ao2o22_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of ao2o22_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0000011101110111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_ao2o22_x2_FTGS of ao2o22_x2 is
   for FTGS
   end for;
end CFG_ao2o22_x2_FTGS;


----- CELL ao2o22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity ao2o22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.696 ns;
      tpdi0_q_F                      :	Time := 0.569 ns;
      tpdi1_q_R                      :	Time := 0.637 ns;
      tpdi1_q_F                      :	Time := 0.666 ns;
      tpdi2_q_R                      :	Time := 0.554 ns;
      tpdi2_q_F                      :	Time := 0.744 ns;
      tpdi3_q_R                      :	Time := 0.606 ns;
      tpdi3_q_F                      :	Time := 0.639 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end ao2o22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of ao2o22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0000011101110111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_ao2o22_x4_FTGS of ao2o22_x4 is
   for FTGS
   end for;
end CFG_ao2o22_x4_FTGS;


----- CELL ao22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity ao22_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.558 ns;
      tpdi0_q_F                      :	Time := 0.447 ns;
      tpdi1_q_R                      :	Time := 0.493 ns;
      tpdi1_q_F                      :	Time := 0.526 ns;
      tpdi2_q_R                      :	Time := 0.420 ns;
      tpdi2_q_F                      :	Time := 0.425 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end ao22_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of ao22_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "00010101",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_ao22_x2_FTGS of ao22_x2 is
   for FTGS
   end for;
end CFG_ao22_x2_FTGS;


----- CELL ao22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity ao22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.674 ns;
      tpdi0_q_F                      :	Time := 0.552 ns;
      tpdi1_q_R                      :	Time := 0.615 ns;
      tpdi1_q_F                      :	Time := 0.647 ns;
      tpdi2_q_R                      :	Time := 0.526 ns;
      tpdi2_q_F                      :	Time := 0.505 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end ao22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of ao22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "00010101",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_ao22_x4_FTGS of ao22_x4 is
   for FTGS
   end for;
end CFG_ao22_x4_FTGS;


----- CELL buf_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity buf_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_q_R                       :	Time := 0.409 ns;
      tpdi_q_F                       :	Time := 0.391 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end buf_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of buf_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "01",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i q",
		delay_param =>
			( 0 => (tpdi_q_R, tpdi_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => q);


end FTGS;

configuration CFG_buf_x2_FTGS of buf_x2 is
   for FTGS
   end for;
end CFG_buf_x2_FTGS;


----- CELL buf_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity buf_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_q_R                       :	Time := 0.379 ns;
      tpdi_q_F                       :	Time := 0.409 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end buf_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of buf_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "01",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i q",
		delay_param =>
			( 0 => (tpdi_q_R, tpdi_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => q);


end FTGS;

configuration CFG_buf_x4_FTGS of buf_x4 is
   for FTGS
   end for;
end CFG_buf_x4_FTGS;


----- CELL buf_x8 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity buf_x8 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_q_R                       :	Time := 0.343 ns;
      tpdi_q_F                       :	Time := 0.396 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end buf_x8;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of buf_x8 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "01",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i q",
		delay_param =>
			( 0 => (tpdi_q_R, tpdi_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => q);


end FTGS;

configuration CFG_buf_x8_FTGS of buf_x8 is
   for FTGS
   end for;
end CFG_buf_x8_FTGS;


----- CELL inv_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity inv_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_nq_R                      :	Time := 0.101 ns;
      tpdi_nq_F                      :	Time := 0.139 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end inv_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of inv_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "10",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i nq",
		delay_param =>
			( 0 => (tpdi_nq_R, tpdi_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => nq);


end FTGS;

configuration CFG_inv_x1_FTGS of inv_x1 is
   for FTGS
   end for;
end CFG_inv_x1_FTGS;


----- CELL inv_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity inv_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_nq_R                      :	Time := 0.069 ns;
      tpdi_nq_F                      :	Time := 0.163 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end inv_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of inv_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "10",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i nq",
		delay_param =>
			( 0 => (tpdi_nq_R, tpdi_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => nq);


end FTGS;

configuration CFG_inv_x2_FTGS of inv_x2 is
   for FTGS
   end for;
end CFG_inv_x2_FTGS;


----- CELL inv_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity inv_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_nq_R                      :	Time := 0.071 ns;
      tpdi_nq_F                      :	Time := 0.143 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end inv_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of inv_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "10",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i nq",
		delay_param =>
			( 0 => (tpdi_nq_R, tpdi_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => nq);


end FTGS;

configuration CFG_inv_x4_FTGS of inv_x4 is
   for FTGS
   end for;
end CFG_inv_x4_FTGS;


----- CELL inv_x8 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity inv_x8 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi_nq_R                      :	Time := 0.086 ns;
      tpdi_nq_F                      :	Time := 0.133 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end inv_x8;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of inv_x8 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is 
	"U2/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is 
	"U2/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : TLU
	generic map(
		N => 1,
		TruthTable => "10",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i nq",
		delay_param =>
			( 0 => (tpdi_nq_R, tpdi_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "X",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Output => nq);


end FTGS;

configuration CFG_inv_x8_FTGS of inv_x8 is
   for FTGS
   end for;
end CFG_inv_x8_FTGS;


----- CELL mx2_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity mx2_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_q_R                     :	Time := 0.484 ns;
      tpdcmd_q_F                     :	Time := 0.522 ns;
      tpdi0_q_R                      :	Time := 0.451 ns;
      tpdi0_q_F                      :	Time := 0.469 ns;
      tpdi1_q_R                      :	Time := 0.451 ns;
      tpdi1_q_F                      :	Time := 0.469 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end mx2_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of mx2_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "00110101",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd i0 i1 q",
		delay_param =>
			((tpdcmd_q_R, tpdcmd_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_mx2_x2_FTGS of mx2_x2 is
   for FTGS
   end for;
end CFG_mx2_x2_FTGS;


----- CELL mx2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity mx2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_q_R                     :	Time := 0.615 ns;
      tpdcmd_q_F                     :	Time := 0.647 ns;
      tpdi0_q_R                      :	Time := 0.564 ns;
      tpdi0_q_F                      :	Time := 0.576 ns;
      tpdi1_q_R                      :	Time := 0.564 ns;
      tpdi1_q_F                      :	Time := 0.576 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end mx2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of mx2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "00110101",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd i0 i1 q",
		delay_param =>
			((tpdcmd_q_R, tpdcmd_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_mx2_x4_FTGS of mx2_x4 is
   for FTGS
   end for;
end CFG_mx2_x4_FTGS;


----- CELL mx3_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity mx3_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd0_q_R                    :	Time := 0.573 ns;
      tpdcmd0_q_F                    :	Time := 0.680 ns;
      tpdcmd1_q_R                    :	Time := 0.664 ns;
      tpdcmd1_q_F                    :	Time := 0.817 ns;
      tpdi0_q_R                      :	Time := 0.538 ns;
      tpdi0_q_F                      :	Time := 0.658 ns;
      tpdi1_q_R                      :	Time := 0.654 ns;
      tpdi1_q_F                      :	Time := 0.808 ns;
      tpdi2_q_R                      :	Time := 0.654 ns;
      tpdi2_q_F                      :	Time := 0.808 ns;
      twdcmd0_R                      :	Time := 0.000 ns;
      twdcmd0_F                      :	Time := 0.000 ns;
      twdcmd1_R                      :	Time := 0.000 ns;
      twdcmd1_F                      :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end mx3_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of mx3_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd1_q_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd1_q_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd0_q_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd0_q_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd0_R, twdcmd0_F, twdcmd0_R, twdcmd0_R, twdcmd0_F, twdcmd0_F))
	port map( Input => cmd0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdcmd1_R, twdcmd1_F, twdcmd1_R, twdcmd1_R, twdcmd1_F, twdcmd1_F))
	port map( Input => cmd1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "00001111000011110101010100110011",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd0 cmd1 i0 i1 i2 q",
		delay_param =>
			((tpdcmd0_q_R, tpdcmd0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd1_q_R, tpdcmd1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => q);


end FTGS;

configuration CFG_mx3_x2_FTGS of mx3_x2 is
   for FTGS
   end for;
end CFG_mx3_x2_FTGS;


----- CELL mx3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity mx3_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd0_q_R                    :	Time := 0.683 ns;
      tpdcmd0_q_F                    :	Time := 0.779 ns;
      tpdcmd1_q_R                    :	Time := 0.792 ns;
      tpdcmd1_q_F                    :	Time := 0.967 ns;
      tpdi0_q_R                      :	Time := 0.640 ns;
      tpdi0_q_F                      :	Time := 0.774 ns;
      tpdi1_q_R                      :	Time := 0.770 ns;
      tpdi1_q_F                      :	Time := 0.948 ns;
      tpdi2_q_R                      :	Time := 0.770 ns;
      tpdi2_q_F                      :	Time := 0.948 ns;
      twdcmd0_R                      :	Time := 0.000 ns;
      twdcmd0_F                      :	Time := 0.000 ns;
      twdcmd1_R                      :	Time := 0.000 ns;
      twdcmd1_F                      :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end mx3_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of mx3_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd1_q_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd1_q_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd0_q_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd0_q_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd0_R, twdcmd0_F, twdcmd0_R, twdcmd0_R, twdcmd0_F, twdcmd0_F))
	port map( Input => cmd0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdcmd1_R, twdcmd1_F, twdcmd1_R, twdcmd1_R, twdcmd1_F, twdcmd1_F))
	port map( Input => cmd1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "00001111000011110101010100110011",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd0 cmd1 i0 i1 i2 q",
		delay_param =>
			((tpdcmd0_q_R, tpdcmd0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd1_q_R, tpdcmd1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => q);


end FTGS;

configuration CFG_mx3_x4_FTGS of mx3_x4 is
   for FTGS
   end for;
end CFG_mx3_x4_FTGS;


----- CELL na2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity na2_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.059 ns;
      tpdi0_nq_F                     :	Time := 0.288 ns;
      tpdi1_nq_R                     :	Time := 0.111 ns;
      tpdi1_nq_F                     :	Time := 0.234 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end na2_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of na2_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => nq);


end FTGS;

configuration CFG_na2_x1_FTGS of na2_x1 is
   for FTGS
   end for;
end CFG_na2_x1_FTGS;


----- CELL na2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity na2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.412 ns;
      tpdi0_nq_F                     :	Time := 0.552 ns;
      tpdi1_nq_R                     :	Time := 0.353 ns;
      tpdi1_nq_F                     :	Time := 0.601 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end na2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of na2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => nq);


end FTGS;

configuration CFG_na2_x4_FTGS of na2_x4 is
   for FTGS
   end for;
end CFG_na2_x4_FTGS;


----- CELL na3_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity na3_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.119 ns;
      tpdi0_nq_F                     :	Time := 0.363 ns;
      tpdi1_nq_R                     :	Time := 0.171 ns;
      tpdi1_nq_F                     :	Time := 0.316 ns;
      tpdi2_nq_R                     :	Time := 0.193 ns;
      tpdi2_nq_F                     :	Time := 0.265 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end na3_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of na3_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "11111110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_na3_x1_FTGS of na3_x1 is
   for FTGS
   end for;
end CFG_na3_x1_FTGS;


----- CELL na3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity na3_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.556 ns;
      tpdi0_nq_F                     :	Time := 0.601 ns;
      tpdi1_nq_R                     :	Time := 0.460 ns;
      tpdi1_nq_F                     :	Time := 0.691 ns;
      tpdi2_nq_R                     :	Time := 0.519 ns;
      tpdi2_nq_F                     :	Time := 0.647 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end na3_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of na3_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "11111110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_na3_x4_FTGS of na3_x4 is
   for FTGS
   end for;
end CFG_na3_x4_FTGS;


----- CELL na4_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity na4_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.179 ns;
      tpdi0_nq_F                     :	Time := 0.438 ns;
      tpdi1_nq_R                     :	Time := 0.237 ns;
      tpdi1_nq_F                     :	Time := 0.395 ns;
      tpdi2_nq_R                     :	Time := 0.269 ns;
      tpdi2_nq_F                     :	Time := 0.350 ns;
      tpdi3_nq_R                     :	Time := 0.282 ns;
      tpdi3_nq_F                     :	Time := 0.302 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end na4_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of na4_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1111111111111110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_na4_x1_FTGS of na4_x1 is
   for FTGS
   end for;
end CFG_na4_x1_FTGS;


----- CELL na4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity na4_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.578 ns;
      tpdi0_nq_F                     :	Time := 0.771 ns;
      tpdi1_nq_R                     :	Time := 0.643 ns;
      tpdi1_nq_F                     :	Time := 0.731 ns;
      tpdi2_nq_R                     :	Time := 0.681 ns;
      tpdi2_nq_F                     :	Time := 0.689 ns;
      tpdi3_nq_R                     :	Time := 0.703 ns;
      tpdi3_nq_F                     :	Time := 0.644 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end na4_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of na4_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1111111111111110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_na4_x4_FTGS of na4_x4 is
   for FTGS
   end for;
end CFG_na4_x4_FTGS;


----- CELL nao2o22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nao2o22_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.294 ns;
      tpdi0_nq_F                     :	Time := 0.226 ns;
      tpdi1_nq_R                     :	Time := 0.218 ns;
      tpdi1_nq_F                     :	Time := 0.287 ns;
      tpdi2_nq_R                     :	Time := 0.237 ns;
      tpdi2_nq_F                     :	Time := 0.307 ns;
      tpdi3_nq_R                     :	Time := 0.174 ns;
      tpdi3_nq_F                     :	Time := 0.382 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nao2o22_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nao2o22_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1111100010001000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_nao2o22_x1_FTGS of nao2o22_x1 is
   for FTGS
   end for;
end CFG_nao2o22_x1_FTGS;


----- CELL nao2o22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nao2o22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.734 ns;
      tpdi0_nq_F                     :	Time := 0.644 ns;
      tpdi1_nq_R                     :	Time := 0.666 ns;
      tpdi1_nq_F                     :	Time := 0.717 ns;
      tpdi2_nq_R                     :	Time := 0.664 ns;
      tpdi2_nq_F                     :	Time := 0.721 ns;
      tpdi3_nq_R                     :	Time := 0.607 ns;
      tpdi3_nq_F                     :	Time := 0.807 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nao2o22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nao2o22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1111100010001000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_nao2o22_x4_FTGS of nao2o22_x4 is
   for FTGS
   end for;
end CFG_nao2o22_x4_FTGS;


----- CELL nao22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nao22_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.294 ns;
      tpdi0_nq_F                     :	Time := 0.226 ns;
      tpdi1_nq_R                     :	Time := 0.218 ns;
      tpdi1_nq_F                     :	Time := 0.287 ns;
      tpdi2_nq_R                     :	Time := 0.165 ns;
      tpdi2_nq_F                     :	Time := 0.238 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nao22_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nao22_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "11101010",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_nao22_x1_FTGS of nao22_x1 is
   for FTGS
   end for;
end CFG_nao22_x1_FTGS;


----- CELL nao22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nao22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.732 ns;
      tpdi0_nq_F                     :	Time := 0.650 ns;
      tpdi1_nq_R                     :	Time := 0.664 ns;
      tpdi1_nq_F                     :	Time := 0.723 ns;
      tpdi2_nq_R                     :	Time := 0.596 ns;
      tpdi2_nq_F                     :	Time := 0.636 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nao22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nao22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "11101010",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_nao22_x4_FTGS of nao22_x4 is
   for FTGS
   end for;
end CFG_nao22_x4_FTGS;


----- CELL nmx2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nmx2_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_nq_R                    :	Time := 0.218 ns;
      tpdcmd_nq_F                    :	Time := 0.287 ns;
      tpdi0_nq_R                     :	Time := 0.217 ns;
      tpdi0_nq_F                     :	Time := 0.256 ns;
      tpdi1_nq_R                     :	Time := 0.217 ns;
      tpdi1_nq_F                     :	Time := 0.256 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nmx2_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nmx2_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "11001010",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd i0 i1 nq",
		delay_param =>
			((tpdcmd_nq_R, tpdcmd_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_nmx2_x1_FTGS of nmx2_x1 is
   for FTGS
   end for;
end CFG_nmx2_x1_FTGS;


----- CELL nmx2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nmx2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_nq_R                    :	Time := 0.632 ns;
      tpdcmd_nq_F                    :	Time := 0.708 ns;
      tpdi0_nq_R                     :	Time := 0.610 ns;
      tpdi0_nq_F                     :	Time := 0.653 ns;
      tpdi1_nq_R                     :	Time := 0.610 ns;
      tpdi1_nq_F                     :	Time := 0.653 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nmx2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nmx2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "11001010",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd i0 i1 nq",
		delay_param =>
			((tpdcmd_nq_R, tpdcmd_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_nmx2_x4_FTGS of nmx2_x4 is
   for FTGS
   end for;
end CFG_nmx2_x4_FTGS;


----- CELL nmx3_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nmx3_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd0_nq_R                   :	Time := 0.356 ns;
      tpdcmd0_nq_F                   :	Time := 0.495 ns;
      tpdcmd1_nq_R                   :	Time := 0.414 ns;
      tpdcmd1_nq_F                   :	Time := 0.566 ns;
      tpdi0_nq_R                     :	Time := 0.315 ns;
      tpdi0_nq_F                     :	Time := 0.441 ns;
      tpdi1_nq_R                     :	Time := 0.429 ns;
      tpdi1_nq_F                     :	Time := 0.582 ns;
      tpdi2_nq_R                     :	Time := 0.429 ns;
      tpdi2_nq_F                     :	Time := 0.582 ns;
      twdcmd0_R                      :	Time := 0.000 ns;
      twdcmd0_F                      :	Time := 0.000 ns;
      twdcmd1_R                      :	Time := 0.000 ns;
      twdcmd1_F                      :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nmx3_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nmx3_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd0_R, twdcmd0_F, twdcmd0_R, twdcmd0_R, twdcmd0_F, twdcmd0_F))
	port map( Input => cmd0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdcmd1_R, twdcmd1_F, twdcmd1_R, twdcmd1_R, twdcmd1_F, twdcmd1_F))
	port map( Input => cmd1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "11110000111100001010101011001100",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd0 cmd1 i0 i1 i2 nq",
		delay_param =>
			((tpdcmd0_nq_R, tpdcmd0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd1_nq_R, tpdcmd1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => nq);


end FTGS;

configuration CFG_nmx3_x1_FTGS of nmx3_x1 is
   for FTGS
   end for;
end CFG_nmx3_x1_FTGS;


----- CELL nmx3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nmx3_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd0_nq_R                   :	Time := 0.790 ns;
      tpdcmd0_nq_F                   :	Time := 0.936 ns;
      tpdcmd1_nq_R                   :	Time := 0.866 ns;
      tpdcmd1_nq_F                   :	Time := 1.048 ns;
      tpdi0_nq_R                     :	Time := 0.748 ns;
      tpdi0_nq_F                     :	Time := 0.900 ns;
      tpdi1_nq_R                     :	Time := 0.869 ns;
      tpdi1_nq_F                     :	Time := 1.053 ns;
      tpdi2_nq_R                     :	Time := 0.869 ns;
      tpdi2_nq_F                     :	Time := 1.053 ns;
      twdcmd0_R                      :	Time := 0.000 ns;
      twdcmd0_F                      :	Time := 0.000 ns;
      twdcmd1_R                      :	Time := 0.000 ns;
      twdcmd1_F                      :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nmx3_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nmx3_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdcmd0_R, twdcmd0_F, twdcmd0_R, twdcmd0_R, twdcmd0_F, twdcmd0_F))
	port map( Input => cmd0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdcmd1_R, twdcmd1_F, twdcmd1_R, twdcmd1_R, twdcmd1_F, twdcmd1_F))
	port map( Input => cmd1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "11110000111100001010101011001100",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "cmd0 cmd1 i0 i1 i2 nq",
		delay_param =>
			((tpdcmd0_nq_R, tpdcmd0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd1_nq_R, tpdcmd1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => nq);


end FTGS;

configuration CFG_nmx3_x4_FTGS of nmx3_x4 is
   for FTGS
   end for;
end CFG_nmx3_x4_FTGS;


----- CELL no2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity no2_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.298 ns;
      tpdi0_nq_F                     :	Time := 0.121 ns;
      tpdi1_nq_R                     :	Time := 0.193 ns;
      tpdi1_nq_F                     :	Time := 0.161 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end no2_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of no2_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => nq);


end FTGS;

configuration CFG_no2_x1_FTGS of no2_x1 is
   for FTGS
   end for;
end CFG_no2_x1_FTGS;


----- CELL no2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity no2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.618 ns;
      tpdi0_nq_F                     :	Time := 0.447 ns;
      tpdi1_nq_R                     :	Time := 0.522 ns;
      tpdi1_nq_F                     :	Time := 0.504 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end no2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of no2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => nq);


end FTGS;

configuration CFG_no2_x4_FTGS of no2_x4 is
   for FTGS
   end for;
end CFG_no2_x4_FTGS;


----- CELL no3_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity no3_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.318 ns;
      tpdi0_nq_F                     :	Time := 0.246 ns;
      tpdi1_nq_R                     :	Time := 0.215 ns;
      tpdi1_nq_F                     :	Time := 0.243 ns;
      tpdi2_nq_R                     :	Time := 0.407 ns;
      tpdi2_nq_F                     :	Time := 0.192 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end no3_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of no3_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "10000000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_no3_x1_FTGS of no3_x1 is
   for FTGS
   end for;
end CFG_no3_x1_FTGS;


----- CELL no3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity no3_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.722 ns;
      tpdi0_nq_F                     :	Time := 0.561 ns;
      tpdi1_nq_R                     :	Time := 0.638 ns;
      tpdi1_nq_F                     :	Time := 0.623 ns;
      tpdi2_nq_R                     :	Time := 0.545 ns;
      tpdi2_nq_F                     :	Time := 0.640 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end no3_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of no3_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "10000000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_no3_x4_FTGS of no3_x4 is
   for FTGS
   end for;
end CFG_no3_x4_FTGS;


----- CELL no4_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity no4_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.330 ns;
      tpdi0_nq_F                     :	Time := 0.340 ns;
      tpdi1_nq_R                     :	Time := 0.230 ns;
      tpdi1_nq_F                     :	Time := 0.320 ns;
      tpdi2_nq_R                     :	Time := 0.419 ns;
      tpdi2_nq_F                     :	Time := 0.333 ns;
      tpdi3_nq_R                     :	Time := 0.499 ns;
      tpdi3_nq_F                     :	Time := 0.271 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end no4_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of no4_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1000000000000000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_no4_x1_FTGS of no4_x1 is
   for FTGS
   end for;
end CFG_no4_x1_FTGS;


----- CELL no4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity no4_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.656 ns;
      tpdi0_nq_F                     :	Time := 0.777 ns;
      tpdi1_nq_R                     :	Time := 0.564 ns;
      tpdi1_nq_F                     :	Time := 0.768 ns;
      tpdi2_nq_R                     :	Time := 0.739 ns;
      tpdi2_nq_F                     :	Time := 0.761 ns;
      tpdi3_nq_R                     :	Time := 0.816 ns;
      tpdi3_nq_F                     :	Time := 0.693 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end no4_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of no4_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1000000000000000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_no4_x4_FTGS of no4_x4 is
   for FTGS
   end for;
end CFG_no4_x4_FTGS;


----- CELL noa2a2a2a24_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2a2a2a24_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.649 ns;
      tpdi0_nq_F                     :	Time := 0.606 ns;
      tpdi1_nq_R                     :	Time := 0.775 ns;
      tpdi1_nq_F                     :	Time := 0.562 ns;
      tpdi2_nq_R                     :	Time := 0.550 ns;
      tpdi2_nq_F                     :	Time := 0.662 ns;
      tpdi3_nq_R                     :	Time := 0.667 ns;
      tpdi3_nq_F                     :	Time := 0.616 ns;
      tpdi4_nq_R                     :	Time := 0.419 ns;
      tpdi4_nq_F                     :	Time := 0.613 ns;
      tpdi5_nq_R                     :	Time := 0.329 ns;
      tpdi5_nq_F                     :	Time := 0.662 ns;
      tpdi6_nq_R                     :	Time := 0.270 ns;
      tpdi6_nq_F                     :	Time := 0.535 ns;
      tpdi7_nq_R                     :	Time := 0.200 ns;
      tpdi7_nq_F                     :	Time := 0.591 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns;
      twdi7_R                        :	Time := 0.000 ns;
      twdi7_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      i7                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2a2a2a24_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2a2a2a24_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi7_nq_F: constant is 
	"U9/delay_param(7)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi7_nq_R: constant is 
	"U9/delay_param(7)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is 
	"U9/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is 
	"U9/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U9/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U9/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U9/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U9/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is 
	"U9/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is 
	"U9/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U9/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U9/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U9/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U9/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U9/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U9/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi7_F: constant is 
	"U8/delay(TRAN_10), " &
	"U8/delay(TRAN_1Z), U8/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi7_R: constant is 
	"U8/delay(TRAN_01), " &
	"U8/delay(TRAN_0Z), U8/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   U8 : WIREBUF
	generic map(delay => (twdi7_R, twdi7_F, twdi7_R, twdi7_R, twdi7_F, twdi7_F))
	port map( Input => i7, Output => connect(7));

   -- Netlist
   U9 : TLU
	generic map(
		N => 8,
		TruthTable => "0001000100011111" &
		              "10101000101010001010100000000000",
		TT_size => (4, 5),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, 7, -1),
		pin_names => "i2 i3 i4 i5 i0 i1 i6 i7 nq",
		delay_param =>
			((tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_nq_R, tpdi5_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_nq_R, tpdi6_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi7_nq_R, tpdi7_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(2), 
		Input(1) => connect(3), 
		Input(2) => connect(4), 
		Input(3) => connect(5), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(6), 
		Input(7) => connect(7), 
		Output => nq);


end FTGS;

configuration CFG_noa2a2a2a24_x1_FTGS of noa2a2a2a24_x1 is
   for FTGS
   end for;
end CFG_noa2a2a2a24_x1_FTGS;


----- CELL noa2a2a2a24_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2a2a2a24_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.966 ns;
      tpdi0_nq_F                     :	Time := 1.049 ns;
      tpdi1_nq_R                     :	Time := 1.097 ns;
      tpdi1_nq_F                     :	Time := 1.005 ns;
      tpdi2_nq_R                     :	Time := 0.867 ns;
      tpdi2_nq_F                     :	Time := 1.106 ns;
      tpdi3_nq_R                     :	Time := 0.990 ns;
      tpdi3_nq_F                     :	Time := 1.061 ns;
      tpdi4_nq_R                     :	Time := 0.748 ns;
      tpdi4_nq_F                     :	Time := 1.061 ns;
      tpdi5_nq_R                     :	Time := 0.649 ns;
      tpdi5_nq_F                     :	Time := 1.109 ns;
      tpdi6_nq_R                     :	Time := 0.606 ns;
      tpdi6_nq_F                     :	Time := 0.999 ns;
      tpdi7_nq_R                     :	Time := 0.525 ns;
      tpdi7_nq_F                     :	Time := 1.052 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns;
      twdi7_R                        :	Time := 0.000 ns;
      twdi7_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      i7                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2a2a2a24_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2a2a2a24_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi7_nq_F: constant is 
	"U9/delay_param(7)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi7_nq_R: constant is 
	"U9/delay_param(7)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is 
	"U9/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is 
	"U9/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U9/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U9/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U9/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U9/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is 
	"U9/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is 
	"U9/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U9/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U9/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U9/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U9/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U9/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U9/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi7_F: constant is 
	"U8/delay(TRAN_10), " &
	"U8/delay(TRAN_1Z), U8/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi7_R: constant is 
	"U8/delay(TRAN_01), " &
	"U8/delay(TRAN_0Z), U8/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   U8 : WIREBUF
	generic map(delay => (twdi7_R, twdi7_F, twdi7_R, twdi7_R, twdi7_F, twdi7_F))
	port map( Input => i7, Output => connect(7));

   -- Netlist
   U9 : TLU
	generic map(
		N => 8,
		TruthTable => "0001000100011111" &
		              "10101000101010001010100000000000",
		TT_size => (4, 5),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, 7, -1),
		pin_names => "i2 i3 i4 i5 i0 i1 i6 i7 nq",
		delay_param =>
			((tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_nq_R, tpdi5_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_nq_R, tpdi6_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi7_nq_R, tpdi7_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(2), 
		Input(1) => connect(3), 
		Input(2) => connect(4), 
		Input(3) => connect(5), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(6), 
		Input(7) => connect(7), 
		Output => nq);


end FTGS;

configuration CFG_noa2a2a2a24_x4_FTGS of noa2a2a2a24_x4 is
   for FTGS
   end for;
end CFG_noa2a2a2a24_x4_FTGS;


----- CELL noa2a2a23_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2a2a23_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.525 ns;
      tpdi0_nq_F                     :	Time := 0.425 ns;
      tpdi1_nq_R                     :	Time := 0.643 ns;
      tpdi1_nq_F                     :	Time := 0.388 ns;
      tpdi2_nq_R                     :	Time := 0.307 ns;
      tpdi2_nq_F                     :	Time := 0.479 ns;
      tpdi3_nq_R                     :	Time := 0.398 ns;
      tpdi3_nq_F                     :	Time := 0.438 ns;
      tpdi4_nq_R                     :	Time := 0.250 ns;
      tpdi4_nq_F                     :	Time := 0.416 ns;
      tpdi5_nq_R                     :	Time := 0.178 ns;
      tpdi5_nq_F                     :	Time := 0.464 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2a2a23_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2a2a23_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U7/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U7/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U7/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U7/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is 
	"U7/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is 
	"U7/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U7/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U7/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U7/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U7/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U7/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U7/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   -- Netlist
   U7 : TLU
	generic map(
		N => 6,
		TruthTable => "0001000100011111" &
		              "10101000",
		TT_size => (4, 3),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, -1),
		pin_names => "i0 i1 i4 i5 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_nq_R, tpdi5_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(4), 
		Input(3) => connect(5), 
		Input(4) => connect(2), 
		Input(5) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_noa2a2a23_x1_FTGS of noa2a2a23_x1 is
   for FTGS
   end for;
end CFG_noa2a2a23_x1_FTGS;


----- CELL noa2a2a23_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2a2a23_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.834 ns;
      tpdi0_nq_F                     :	Time := 0.814 ns;
      tpdi1_nq_R                     :	Time := 0.955 ns;
      tpdi1_nq_F                     :	Time := 0.778 ns;
      tpdi2_nq_R                     :	Time := 0.620 ns;
      tpdi2_nq_F                     :	Time := 0.873 ns;
      tpdi3_nq_R                     :	Time := 0.716 ns;
      tpdi3_nq_F                     :	Time := 0.833 ns;
      tpdi4_nq_R                     :	Time := 0.574 ns;
      tpdi4_nq_F                     :	Time := 0.819 ns;
      tpdi5_nq_R                     :	Time := 0.496 ns;
      tpdi5_nq_F                     :	Time := 0.865 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2a2a23_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2a2a23_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U7/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U7/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U7/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U7/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is 
	"U7/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is 
	"U7/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U7/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U7/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U7/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U7/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U7/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U7/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   -- Netlist
   U7 : TLU
	generic map(
		N => 6,
		TruthTable => "0001000100011111" &
		              "10101000",
		TT_size => (4, 3),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, -1),
		pin_names => "i0 i1 i4 i5 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_nq_R, tpdi5_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(4), 
		Input(3) => connect(5), 
		Input(4) => connect(2), 
		Input(5) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_noa2a2a23_x4_FTGS of noa2a2a23_x4 is
   for FTGS
   end for;
end CFG_noa2a2a23_x4_FTGS;


----- CELL noa2a22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2a22_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.151 ns;
      tpdi0_nq_F                     :	Time := 0.327 ns;
      tpdi1_nq_R                     :	Time := 0.218 ns;
      tpdi1_nq_F                     :	Time := 0.287 ns;
      tpdi2_nq_R                     :	Time := 0.284 ns;
      tpdi2_nq_F                     :	Time := 0.289 ns;
      tpdi3_nq_R                     :	Time := 0.372 ns;
      tpdi3_nq_F                     :	Time := 0.256 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2a22_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2a22_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1110111011100000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_noa2a22_x1_FTGS of noa2a22_x1 is
   for FTGS
   end for;
end CFG_noa2a22_x1_FTGS;


----- CELL noa2a22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2a22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.562 ns;
      tpdi0_nq_F                     :	Time := 0.745 ns;
      tpdi1_nq_R                     :	Time := 0.646 ns;
      tpdi1_nq_F                     :	Time := 0.714 ns;
      tpdi2_nq_R                     :	Time := 0.701 ns;
      tpdi2_nq_F                     :	Time := 0.703 ns;
      tpdi3_nq_R                     :	Time := 0.805 ns;
      tpdi3_nq_F                     :	Time := 0.677 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2a22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2a22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "1110111011100000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => nq);


end FTGS;

configuration CFG_noa2a22_x4_FTGS of noa2a22_x4 is
   for FTGS
   end for;
end CFG_noa2a22_x4_FTGS;


----- CELL noa2ao222_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2ao222_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.348 ns;
      tpdi0_nq_F                     :	Time := 0.422 ns;
      tpdi1_nq_R                     :	Time := 0.440 ns;
      tpdi1_nq_F                     :	Time := 0.378 ns;
      tpdi2_nq_R                     :	Time := 0.186 ns;
      tpdi2_nq_F                     :	Time := 0.473 ns;
      tpdi3_nq_R                     :	Time := 0.256 ns;
      tpdi3_nq_F                     :	Time := 0.459 ns;
      tpdi4_nq_R                     :	Time := 0.240 ns;
      tpdi4_nq_F                     :	Time := 0.309 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2ao222_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2ao222_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "11101010111010101110101000000000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 i4 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => nq);


end FTGS;

configuration CFG_noa2ao222_x1_FTGS of noa2ao222_x1 is
   for FTGS
   end for;
end CFG_noa2ao222_x1_FTGS;


----- CELL noa2ao222_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa2ao222_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.684 ns;
      tpdi0_nq_F                     :	Time := 0.801 ns;
      tpdi1_nq_R                     :	Time := 0.780 ns;
      tpdi1_nq_F                     :	Time := 0.758 ns;
      tpdi2_nq_R                     :	Time := 0.638 ns;
      tpdi2_nq_F                     :	Time := 0.809 ns;
      tpdi3_nq_R                     :	Time := 0.732 ns;
      tpdi3_nq_F                     :	Time := 0.795 ns;
      tpdi4_nq_R                     :	Time := 0.718 ns;
      tpdi4_nq_F                     :	Time := 0.664 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa2ao222_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa2ao222_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "11101010111010101110101000000000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 i4 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => nq);


end FTGS;

configuration CFG_noa2ao222_x4_FTGS of noa2ao222_x4 is
   for FTGS
   end for;
end CFG_noa2ao222_x4_FTGS;


----- CELL noa3ao322_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa3ao322_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.396 ns;
      tpdi0_nq_F                     :	Time := 0.616 ns;
      tpdi1_nq_R                     :	Time := 0.486 ns;
      tpdi1_nq_F                     :	Time := 0.552 ns;
      tpdi2_nq_R                     :	Time := 0.546 ns;
      tpdi2_nq_F                     :	Time := 0.488 ns;
      tpdi3_nq_R                     :	Time := 0.196 ns;
      tpdi3_nq_F                     :	Time := 0.599 ns;
      tpdi4_nq_R                     :	Time := 0.264 ns;
      tpdi4_nq_F                     :	Time := 0.608 ns;
      tpdi5_nq_R                     :	Time := 0.328 ns;
      tpdi5_nq_F                     :	Time := 0.581 ns;
      tpdi6_nq_R                     :	Time := 0.246 ns;
      tpdi6_nq_F                     :	Time := 0.311 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa3ao322_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa3ao322_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U8/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U8/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U8/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U8/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U8/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U8/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is 
	"U8/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is 
	"U8/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is 
	"U8/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is 
	"U8/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U8/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U8/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U8/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U8/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   -- Netlist
   U8 : TLU
	generic map(
		N => 7,
		TruthTable => "0001010101010101" &
		              "1010101010101000",
		TT_size => (4, 4),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, -1),
		pin_names => "i3 i4 i5 i6 i0 i1 i2 nq",
		delay_param =>
			((tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_nq_R, tpdi5_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_nq_R, tpdi6_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(3), 
		Input(1) => connect(4), 
		Input(2) => connect(5), 
		Input(3) => connect(6), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_noa3ao322_x1_FTGS of noa3ao322_x1 is
   for FTGS
   end for;
end CFG_noa3ao322_x1_FTGS;


----- CELL noa3ao322_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa3ao322_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.819 ns;
      tpdi0_nq_F                     :	Time := 0.987 ns;
      tpdi1_nq_R                     :	Time := 0.914 ns;
      tpdi1_nq_F                     :	Time := 0.931 ns;
      tpdi2_nq_R                     :	Time := 0.990 ns;
      tpdi2_nq_F                     :	Time := 0.874 ns;
      tpdi3_nq_R                     :	Time := 0.729 ns;
      tpdi3_nq_F                     :	Time := 0.926 ns;
      tpdi4_nq_R                     :	Time := 0.821 ns;
      tpdi4_nq_F                     :	Time := 0.924 ns;
      tpdi5_nq_R                     :	Time := 0.907 ns;
      tpdi5_nq_F                     :	Time := 0.900 ns;
      tpdi6_nq_R                     :	Time := 0.738 ns;
      tpdi6_nq_F                     :	Time := 0.718 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa3ao322_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa3ao322_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U8/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U8/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U8/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U8/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U8/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U8/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is 
	"U8/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is 
	"U8/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is 
	"U8/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is 
	"U8/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is 
	"U8/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is 
	"U8/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is 
	"U8/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is 
	"U8/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   -- Netlist
   U8 : TLU
	generic map(
		N => 7,
		TruthTable => "0001010101010101" &
		              "1010101010101000",
		TT_size => (4, 4),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, -1),
		pin_names => "i3 i4 i5 i6 i0 i1 i2 nq",
		delay_param =>
			((tpdi3_nq_R, tpdi3_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_nq_R, tpdi4_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_nq_R, tpdi5_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_nq_R, tpdi6_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(3), 
		Input(1) => connect(4), 
		Input(2) => connect(5), 
		Input(3) => connect(6), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_noa3ao322_x4_FTGS of noa3ao322_x4 is
   for FTGS
   end for;
end CFG_noa3ao322_x4_FTGS;


----- CELL noa22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa22_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.151 ns;
      tpdi0_nq_F                     :	Time := 0.327 ns;
      tpdi1_nq_R                     :	Time := 0.218 ns;
      tpdi1_nq_F                     :	Time := 0.287 ns;
      tpdi2_nq_R                     :	Time := 0.218 ns;
      tpdi2_nq_F                     :	Time := 0.241 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa22_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa22_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "10101000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_noa22_x1_FTGS of noa22_x1 is
   for FTGS
   end for;
end CFG_noa22_x1_FTGS;


----- CELL noa22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity noa22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.550 ns;
      tpdi0_nq_F                     :	Time := 0.740 ns;
      tpdi1_nq_R                     :	Time := 0.643 ns;
      tpdi1_nq_F                     :	Time := 0.709 ns;
      tpdi2_nq_R                     :	Time := 0.610 ns;
      tpdi2_nq_F                     :	Time := 0.646 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end noa22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of noa22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "10101000",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_nq_R, tpdi2_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => nq);


end FTGS;

configuration CFG_noa22_x4_FTGS of noa22_x4 is
   for FTGS
   end for;
end CFG_noa22_x4_FTGS;


----- CELL nts_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nts_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_nq_R                    :	Time := 0.249 ns;
      tpdcmd_nq_F                    :	Time := 0.041 ns;
      tpdcmd_nq_LZ                   :	Time := 0.249 ns;
      tpdcmd_nq_HZ                   :	Time := 0.041 ns;
      tpdi_nq_R                      :	Time := 0.169 ns;
      tpdi_nq_F                      :	Time := 0.201 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nts_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nts_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdcmd_nq_HZ: constant is 
	"U3/delay_param(1)(TRAN_1Z)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_LZ: constant is 
	"U3/delay_param(1)(TRAN_0Z)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10), " &
	"U3/delay_param(1)(TRAN_Z0)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01), " &
	"U3/delay_param(1)(TRAN_Z1)";
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(1));

   U2 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "Z1Z0",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i cmd nq",
		delay_param =>
			((tpdi_nq_R, tpdi_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd_nq_R, tpdcmd_nq_F, tpdcmd_nq_LZ, tpdcmd_nq_R, tpdcmd_nq_HZ, tpdcmd_nq_F)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(1), 
		Input(1) => connect(0), 
		Output => nq);


end FTGS;

configuration CFG_nts_x1_FTGS of nts_x1 is
   for FTGS
   end for;
end CFG_nts_x1_FTGS;


----- CELL nts_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nts_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_nq_R                    :	Time := 0.330 ns;
      tpdcmd_nq_F                    :	Time := 0.033 ns;
      tpdcmd_nq_LZ                   :	Time := 0.330 ns;
      tpdcmd_nq_HZ                   :	Time := 0.033 ns;
      tpdi_nq_R                      :	Time := 0.167 ns;
      tpdi_nq_F                      :	Time := 0.201 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nts_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nts_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdcmd_nq_HZ: constant is 
	"U3/delay_param(1)(TRAN_1Z)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_LZ: constant is 
	"U3/delay_param(1)(TRAN_0Z)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10), " &
	"U3/delay_param(1)(TRAN_Z0)";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01), " &
	"U3/delay_param(1)(TRAN_Z1)";
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(1));

   U2 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "Z1Z0",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i cmd nq",
		delay_param =>
			((tpdi_nq_R, tpdi_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd_nq_R, tpdcmd_nq_F, tpdcmd_nq_LZ, tpdcmd_nq_R, tpdcmd_nq_HZ, tpdcmd_nq_F)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(1), 
		Input(1) => connect(0), 
		Output => nq);


end FTGS;

configuration CFG_nts_x2_FTGS of nts_x2 is
   for FTGS
   end for;
end CFG_nts_x2_FTGS;


----- CELL nxr2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nxr2_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.288 ns;
      tpdi0_nq_F                     :	Time := 0.293 ns;
      tpdi1_nq_R                     :	Time := 0.156 ns;
      tpdi1_nq_F                     :	Time := 0.327 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nxr2_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nxr2_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => nq);


end FTGS;

configuration CFG_nxr2_x1_FTGS of nxr2_x1 is
   for FTGS
   end for;
end CFG_nxr2_x1_FTGS;


----- CELL nxr2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity nxr2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_nq_R                     :	Time := 0.522 ns;
      tpdi0_nq_F                     :	Time := 0.553 ns;
      tpdi1_nq_R                     :	Time := 0.553 ns;
      tpdi1_nq_F                     :	Time := 0.542 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end nxr2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of nxr2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1001",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 nq",
		delay_param =>
			((tpdi0_nq_R, tpdi0_nq_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_nq_R, tpdi1_nq_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => nq);


end FTGS;

configuration CFG_nxr2_x4_FTGS of nxr2_x4 is
   for FTGS
   end for;
end CFG_nxr2_x4_FTGS;


----- CELL o2_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity o2_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.406 ns;
      tpdi0_q_F                      :	Time := 0.310 ns;
      tpdi1_q_R                      :	Time := 0.335 ns;
      tpdi1_q_F                      :	Time := 0.364 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end o2_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of o2_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_o2_x2_FTGS of o2_x2 is
   for FTGS
   end for;
end CFG_o2_x2_FTGS;


----- CELL o2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity o2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.491 ns;
      tpdi0_q_F                      :	Time := 0.394 ns;
      tpdi1_q_R                      :	Time := 0.427 ns;
      tpdi1_q_F                      :	Time := 0.464 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end o2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of o2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_o2_x4_FTGS of o2_x4 is
   for FTGS
   end for;
end CFG_o2_x4_FTGS;


----- CELL o3_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity o3_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.494 ns;
      tpdi0_q_F                      :	Time := 0.407 ns;
      tpdi1_q_R                      :	Time := 0.430 ns;
      tpdi1_q_F                      :	Time := 0.482 ns;
      tpdi2_q_R                      :	Time := 0.360 ns;
      tpdi2_q_F                      :	Time := 0.506 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end o3_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of o3_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "01111111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_o3_x2_FTGS of o3_x2 is
   for FTGS
   end for;
end CFG_o3_x2_FTGS;


----- CELL o3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity o3_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.569 ns;
      tpdi0_q_F                      :	Time := 0.501 ns;
      tpdi1_q_R                      :	Time := 0.510 ns;
      tpdi1_q_F                      :	Time := 0.585 ns;
      tpdi2_q_R                      :	Time := 0.447 ns;
      tpdi2_q_F                      :	Time := 0.622 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end o3_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of o3_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "01111111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_o3_x4_FTGS of o3_x4 is
   for FTGS
   end for;
end CFG_o3_x4_FTGS;


----- CELL o4_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity o4_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.508 ns;
      tpdi0_q_F                      :	Time := 0.601 ns;
      tpdi1_q_R                      :	Time := 0.446 ns;
      tpdi1_q_F                      :	Time := 0.631 ns;
      tpdi2_q_R                      :	Time := 0.567 ns;
      tpdi2_q_F                      :	Time := 0.531 ns;
      tpdi3_q_R                      :	Time := 0.378 ns;
      tpdi3_q_F                      :	Time := 0.626 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end o4_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of o4_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0111111111111111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_o4_x2_FTGS of o4_x2 is
   for FTGS
   end for;
end CFG_o4_x2_FTGS;


----- CELL o4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity o4_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.574 ns;
      tpdi0_q_F                      :	Time := 0.638 ns;
      tpdi1_q_R                      :	Time := 0.492 ns;
      tpdi1_q_F                      :	Time := 0.650 ns;
      tpdi2_q_R                      :	Time := 0.649 ns;
      tpdi2_q_F                      :	Time := 0.611 ns;
      tpdi3_q_R                      :	Time := 0.721 ns;
      tpdi3_q_F                      :	Time := 0.536 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end o4_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of o4_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0111111111111111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_o4_x4_FTGS of o4_x4 is
   for FTGS
   end for;
end CFG_o4_x4_FTGS;


----- CELL oa2a2a2a24_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2a2a2a24_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.780 ns;
      tpdi0_q_F                      :	Time := 0.797 ns;
      tpdi1_q_R                      :	Time := 0.909 ns;
      tpdi1_q_F                      :	Time := 0.753 ns;
      tpdi2_q_R                      :	Time := 0.682 ns;
      tpdi2_q_F                      :	Time := 0.856 ns;
      tpdi3_q_R                      :	Time := 0.803 ns;
      tpdi3_q_F                      :	Time := 0.810 ns;
      tpdi4_q_R                      :	Time := 0.565 ns;
      tpdi4_q_F                      :	Time := 0.813 ns;
      tpdi5_q_R                      :	Time := 0.467 ns;
      tpdi5_q_F                      :	Time := 0.861 ns;
      tpdi6_q_R                      :	Time := 0.426 ns;
      tpdi6_q_F                      :	Time := 0.748 ns;
      tpdi7_q_R                      :	Time := 0.346 ns;
      tpdi7_q_F                      :	Time := 0.800 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns;
      twdi7_R                        :	Time := 0.000 ns;
      twdi7_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      i7                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2a2a2a24_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2a2a2a24_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U9/delay_param(7)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U9/delay_param(7)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U9/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U9/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U9/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U9/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U9/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U9/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi7_q_F: constant is 
	"U9/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi7_q_R: constant is 
	"U9/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is 
	"U9/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is 
	"U9/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is 
	"U9/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is 
	"U9/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U9/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U9/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi7_F: constant is 
	"U8/delay(TRAN_10), " &
	"U8/delay(TRAN_1Z), U8/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi7_R: constant is 
	"U8/delay(TRAN_01), " &
	"U8/delay(TRAN_0Z), U8/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   U8 : WIREBUF
	generic map(delay => (twdi7_R, twdi7_F, twdi7_R, twdi7_R, twdi7_F, twdi7_F))
	port map( Input => i7, Output => connect(7));

   -- Netlist
   U9 : TLU
	generic map(
		N => 8,
		TruthTable => "0001000100011111" &
		              "01010111010101110101011111111111",
		TT_size => (4, 5),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, 7, -1),
		pin_names => "i4 i5 i6 i7 i0 i1 i2 i3 q",
		delay_param =>
			((tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_q_R, tpdi5_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_q_R, tpdi6_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi7_q_R, tpdi7_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(4), 
		Input(1) => connect(5), 
		Input(2) => connect(6), 
		Input(3) => connect(7), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(2), 
		Input(7) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_oa2a2a2a24_x2_FTGS of oa2a2a2a24_x2 is
   for FTGS
   end for;
end CFG_oa2a2a2a24_x2_FTGS;


----- CELL oa2a2a2a24_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2a2a2a24_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.823 ns;
      tpdi0_q_F                      :	Time := 0.879 ns;
      tpdi1_q_R                      :	Time := 0.955 ns;
      tpdi1_q_F                      :	Time := 0.835 ns;
      tpdi2_q_R                      :	Time := 0.726 ns;
      tpdi2_q_F                      :	Time := 0.940 ns;
      tpdi3_q_R                      :	Time := 0.851 ns;
      tpdi3_q_F                      :	Time := 0.895 ns;
      tpdi4_q_R                      :	Time := 0.619 ns;
      tpdi4_q_F                      :	Time := 0.902 ns;
      tpdi5_q_R                      :	Time := 0.515 ns;
      tpdi5_q_F                      :	Time := 0.949 ns;
      tpdi6_q_R                      :	Time := 0.487 ns;
      tpdi6_q_F                      :	Time := 0.845 ns;
      tpdi7_q_R                      :	Time := 0.399 ns;
      tpdi7_q_F                      :	Time := 0.895 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns;
      twdi7_R                        :	Time := 0.000 ns;
      twdi7_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      i7                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2a2a2a24_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2a2a2a24_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U9/delay_param(7)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U9/delay_param(7)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U9/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U9/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U9/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U9/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U9/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U9/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi7_q_F: constant is 
	"U9/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi7_q_R: constant is 
	"U9/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is 
	"U9/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is 
	"U9/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is 
	"U9/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is 
	"U9/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U9/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U9/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi7_F: constant is 
	"U8/delay(TRAN_10), " &
	"U8/delay(TRAN_1Z), U8/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi7_R: constant is 
	"U8/delay(TRAN_01), " &
	"U8/delay(TRAN_0Z), U8/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   U8 : WIREBUF
	generic map(delay => (twdi7_R, twdi7_F, twdi7_R, twdi7_R, twdi7_F, twdi7_F))
	port map( Input => i7, Output => connect(7));

   -- Netlist
   U9 : TLU
	generic map(
		N => 8,
		TruthTable => "0001000100011111" &
		              "01010111010101110101011111111111",
		TT_size => (4, 5),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, 7, -1),
		pin_names => "i4 i5 i6 i7 i0 i1 i2 i3 q",
		delay_param =>
			((tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_q_R, tpdi5_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_q_R, tpdi6_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi7_q_R, tpdi7_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(4), 
		Input(1) => connect(5), 
		Input(2) => connect(6), 
		Input(3) => connect(7), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(2), 
		Input(7) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_oa2a2a2a24_x4_FTGS of oa2a2a2a24_x4 is
   for FTGS
   end for;
end CFG_oa2a2a2a24_x4_FTGS;


----- CELL oa2a2a23_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2a2a23_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.653 ns;
      tpdi0_q_F                      :	Time := 0.578 ns;
      tpdi1_q_R                      :	Time := 0.775 ns;
      tpdi1_q_F                      :	Time := 0.542 ns;
      tpdi2_q_R                      :	Time := 0.441 ns;
      tpdi2_q_F                      :	Time := 0.639 ns;
      tpdi3_q_R                      :	Time := 0.540 ns;
      tpdi3_q_F                      :	Time := 0.600 ns;
      tpdi4_q_R                      :	Time := 0.402 ns;
      tpdi4_q_F                      :	Time := 0.591 ns;
      tpdi5_q_R                      :	Time := 0.321 ns;
      tpdi5_q_F                      :	Time := 0.636 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2a2a23_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2a2a23_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U7/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U7/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U7/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U7/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is 
	"U7/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is 
	"U7/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U7/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U7/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U7/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U7/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U7/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U7/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   -- Netlist
   U7 : TLU
	generic map(
		N => 6,
		TruthTable => "0001000100011111" &
		              "01010111",
		TT_size => (4, 3),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, -1),
		pin_names => "i2 i3 i4 i5 i0 i1 q",
		delay_param =>
			((tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_q_R, tpdi5_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(2), 
		Input(1) => connect(3), 
		Input(2) => connect(4), 
		Input(3) => connect(5), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_oa2a2a23_x2_FTGS of oa2a2a23_x2 is
   for FTGS
   end for;
end CFG_oa2a2a23_x2_FTGS;


----- CELL oa2a2a23_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2a2a23_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.699 ns;
      tpdi0_q_F                      :	Time := 0.648 ns;
      tpdi1_q_R                      :	Time := 0.822 ns;
      tpdi1_q_F                      :	Time := 0.613 ns;
      tpdi2_q_R                      :	Time := 0.493 ns;
      tpdi2_q_F                      :	Time := 0.715 ns;
      tpdi3_q_R                      :	Time := 0.594 ns;
      tpdi3_q_F                      :	Time := 0.677 ns;
      tpdi4_q_R                      :	Time := 0.464 ns;
      tpdi4_q_F                      :	Time := 0.673 ns;
      tpdi5_q_R                      :	Time := 0.379 ns;
      tpdi5_q_F                      :	Time := 0.714 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2a2a23_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2a2a23_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U7/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U7/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U7/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U7/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is 
	"U7/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is 
	"U7/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U7/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U7/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U7/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U7/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U7/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U7/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   -- Netlist
   U7 : TLU
	generic map(
		N => 6,
		TruthTable => "0001000100011111" &
		              "01010111",
		TT_size => (4, 3),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, -1),
		pin_names => "i2 i3 i4 i5 i0 i1 q",
		delay_param =>
			((tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_q_R, tpdi5_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(2), 
		Input(1) => connect(3), 
		Input(2) => connect(4), 
		Input(3) => connect(5), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_oa2a2a23_x4_FTGS of oa2a2a23_x4 is
   for FTGS
   end for;
end CFG_oa2a2a23_x4_FTGS;


----- CELL oa2a22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2a22_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.403 ns;
      tpdi0_q_F                      :	Time := 0.564 ns;
      tpdi1_q_R                      :	Time := 0.495 ns;
      tpdi1_q_F                      :	Time := 0.534 ns;
      tpdi2_q_R                      :	Time := 0.646 ns;
      tpdi2_q_F                      :	Time := 0.487 ns;
      tpdi3_q_R                      :	Time := 0.537 ns;
      tpdi3_q_F                      :	Time := 0.512 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2a22_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2a22_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0001000100011111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_oa2a22_x2_FTGS of oa2a22_x2 is
   for FTGS
   end for;
end CFG_oa2a22_x2_FTGS;


----- CELL oa2a22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2a22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.519 ns;
      tpdi0_q_F                      :	Time := 0.696 ns;
      tpdi1_q_R                      :	Time := 0.624 ns;
      tpdi1_q_F                      :	Time := 0.669 ns;
      tpdi2_q_R                      :	Time := 0.763 ns;
      tpdi2_q_F                      :	Time := 0.596 ns;
      tpdi3_q_R                      :	Time := 0.644 ns;
      tpdi3_q_F                      :	Time := 0.619 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2a22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2a22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U5/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U5/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U5/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U5/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U5/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U5/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   -- Netlist
   U5 : TLU
	generic map(
		N => 4,
		TruthTable => "0001000100011111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Output => q);


end FTGS;

configuration CFG_oa2a22_x4_FTGS of oa2a22_x4 is
   for FTGS
   end for;
end CFG_oa2a22_x4_FTGS;


----- CELL oa2ao222_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2ao222_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.495 ns;
      tpdi0_q_F                      :	Time := 0.581 ns;
      tpdi1_q_R                      :	Time := 0.598 ns;
      tpdi1_q_F                      :	Time := 0.539 ns;
      tpdi2_q_R                      :	Time := 0.464 ns;
      tpdi2_q_F                      :	Time := 0.604 ns;
      tpdi3_q_R                      :	Time := 0.556 ns;
      tpdi3_q_F                      :	Time := 0.578 ns;
      tpdi4_q_R                      :	Time := 0.558 ns;
      tpdi4_q_F                      :	Time := 0.453 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2ao222_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2ao222_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "00010101000101010001010111111111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 i4 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => q);


end FTGS;

configuration CFG_oa2ao222_x2_FTGS of oa2ao222_x2 is
   for FTGS
   end for;
end CFG_oa2ao222_x2_FTGS;


----- CELL oa2ao222_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa2ao222_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.553 ns;
      tpdi0_q_F                      :	Time := 0.657 ns;
      tpdi1_q_R                      :	Time := 0.662 ns;
      tpdi1_q_F                      :	Time := 0.616 ns;
      tpdi2_q_R                      :	Time := 0.552 ns;
      tpdi2_q_F                      :	Time := 0.693 ns;
      tpdi3_q_R                      :	Time := 0.640 ns;
      tpdi3_q_F                      :	Time := 0.660 ns;
      tpdi4_q_R                      :	Time := 0.656 ns;
      tpdi4_q_F                      :	Time := 0.529 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa2ao222_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa2ao222_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U6/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U6/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U6/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U6/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U6/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U6/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U6/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U6/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U6/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U6/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   -- Netlist
   U6 : TLU
	generic map(
		N => 5,
		TruthTable => "00010101000101010001010111111111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 i3 i4 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Input(3) => connect(3), 
		Input(4) => connect(4), 
		Output => q);


end FTGS;

configuration CFG_oa2ao222_x4_FTGS of oa2ao222_x4 is
   for FTGS
   end for;
end CFG_oa2ao222_x4_FTGS;


----- CELL oa3ao322_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa3ao322_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.638 ns;
      tpdi0_q_F                      :	Time := 0.820 ns;
      tpdi1_q_R                      :	Time := 0.735 ns;
      tpdi1_q_F                      :	Time := 0.764 ns;
      tpdi2_q_R                      :	Time := 0.806 ns;
      tpdi2_q_F                      :	Time := 0.707 ns;
      tpdi3_q_R                      :	Time := 0.560 ns;
      tpdi3_q_F                      :	Time := 0.765 ns;
      tpdi4_q_R                      :	Time := 0.649 ns;
      tpdi4_q_F                      :	Time := 0.760 ns;
      tpdi5_q_R                      :	Time := 0.734 ns;
      tpdi5_q_F                      :	Time := 0.734 ns;
      tpdi6_q_R                      :	Time := 0.563 ns;
      tpdi6_q_F                      :	Time := 0.540 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa3ao322_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa3ao322_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U8/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U8/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U8/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U8/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U8/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U8/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is 
	"U8/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is 
	"U8/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is 
	"U8/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is 
	"U8/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U8/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U8/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U8/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U8/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   -- Netlist
   U8 : TLU
	generic map(
		N => 7,
		TruthTable => "0001010101010101" &
		              "0101010101010111",
		TT_size => (4, 4),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, -1),
		pin_names => "i3 i4 i5 i6 i0 i1 i2 q",
		delay_param =>
			((tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_q_R, tpdi5_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_q_R, tpdi6_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(3), 
		Input(1) => connect(4), 
		Input(2) => connect(5), 
		Input(3) => connect(6), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_oa3ao322_x2_FTGS of oa3ao322_x2 is
   for FTGS
   end for;
end CFG_oa3ao322_x2_FTGS;


----- CELL oa3ao322_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa3ao322_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.717 ns;
      tpdi0_q_F                      :	Time := 0.946 ns;
      tpdi1_q_R                      :	Time := 0.818 ns;
      tpdi1_q_F                      :	Time := 0.890 ns;
      tpdi2_q_R                      :	Time := 0.894 ns;
      tpdi2_q_F                      :	Time := 0.834 ns;
      tpdi3_q_R                      :	Time := 0.673 ns;
      tpdi3_q_F                      :	Time := 0.898 ns;
      tpdi4_q_R                      :	Time := 0.758 ns;
      tpdi4_q_F                      :	Time := 0.896 ns;
      tpdi5_q_R                      :	Time := 0.839 ns;
      tpdi5_q_F                      :	Time := 0.865 ns;
      tpdi6_q_R                      :	Time := 0.684 ns;
      tpdi6_q_F                      :	Time := 0.651 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns;
      twdi3_R                        :	Time := 0.000 ns;
      twdi3_F                        :	Time := 0.000 ns;
      twdi4_R                        :	Time := 0.000 ns;
      twdi4_F                        :	Time := 0.000 ns;
      twdi5_R                        :	Time := 0.000 ns;
      twdi5_F                        :	Time := 0.000 ns;
      twdi6_R                        :	Time := 0.000 ns;
      twdi6_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa3ao322_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa3ao322_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U8/delay_param(6)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U8/delay_param(6)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U8/delay_param(5)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U8/delay_param(5)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U8/delay_param(4)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U8/delay_param(4)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is 
	"U8/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is 
	"U8/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is 
	"U8/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is 
	"U8/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is 
	"U8/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is 
	"U8/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is 
	"U8/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is 
	"U8/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi6_F: constant is 
	"U7/delay(TRAN_10), " &
	"U7/delay(TRAN_1Z), U7/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi6_R: constant is 
	"U7/delay(TRAN_01), " &
	"U7/delay(TRAN_0Z), U7/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi5_F: constant is 
	"U6/delay(TRAN_10), " &
	"U6/delay(TRAN_1Z), U6/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi5_R: constant is 
	"U6/delay(TRAN_01), " &
	"U6/delay(TRAN_0Z), U6/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi4_F: constant is 
	"U5/delay(TRAN_10), " &
	"U5/delay(TRAN_1Z), U5/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi4_R: constant is 
	"U5/delay(TRAN_01), " &
	"U5/delay(TRAN_0Z), U5/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi3_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi3_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U'); 

begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdi3_R, twdi3_F, twdi3_R, twdi3_R, twdi3_F, twdi3_F))
	port map( Input => i3, Output => connect(3));

   U5 : WIREBUF
	generic map(delay => (twdi4_R, twdi4_F, twdi4_R, twdi4_R, twdi4_F, twdi4_F))
	port map( Input => i4, Output => connect(4));

   U6 : WIREBUF
	generic map(delay => (twdi5_R, twdi5_F, twdi5_R, twdi5_R, twdi5_F, twdi5_F))
	port map( Input => i5, Output => connect(5));

   U7 : WIREBUF
	generic map(delay => (twdi6_R, twdi6_F, twdi6_R, twdi6_R, twdi6_F, twdi6_F))
	port map( Input => i6, Output => connect(6));

   -- Netlist
   U8 : TLU
	generic map(
		N => 7,
		TruthTable => "0001010101010101" &
		              "0101010101010111",
		TT_size => (4, 4),
		Node_Index => (0, 1, 2, 3, 
		               4, 5, 6, -1),
		pin_names => "i3 i4 i5 i6 i0 i1 i2 q",
		delay_param =>
			((tpdi3_q_R, tpdi3_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi4_q_R, tpdi4_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi5_q_R, tpdi5_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi6_q_R, tpdi6_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXXXXXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(3), 
		Input(1) => connect(4), 
		Input(2) => connect(5), 
		Input(3) => connect(6), 
		Input(4) => connect(0), 
		Input(5) => connect(1), 
		Input(6) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_oa3ao322_x4_FTGS of oa3ao322_x4 is
   for FTGS
   end for;
end CFG_oa3ao322_x4_FTGS;


----- CELL oa22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa22_x2 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.390 ns;
      tpdi0_q_F                      :	Time := 0.555 ns;
      tpdi1_q_R                      :	Time := 0.488 ns;
      tpdi1_q_F                      :	Time := 0.525 ns;
      tpdi2_q_R                      :	Time := 0.438 ns;
      tpdi2_q_F                      :	Time := 0.454 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa22_x2;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa22_x2 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "01010111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_oa22_x2_FTGS of oa22_x2 is
   for FTGS
   end for;
end CFG_oa22_x2_FTGS;


----- CELL oa22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity oa22_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.511 ns;
      tpdi0_q_F                      :	Time := 0.677 ns;
      tpdi1_q_R                      :	Time := 0.615 ns;
      tpdi1_q_F                      :	Time := 0.650 ns;
      tpdi2_q_R                      :	Time := 0.523 ns;
      tpdi2_q_F                      :	Time := 0.571 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdi2_R                        :	Time := 0.000 ns;
      twdi2_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end oa22_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of oa22_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is 
	"U4/delay_param(2)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is 
	"U4/delay_param(2)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U4/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U4/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U4/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U4/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi2_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi2_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdi2_R, twdi2_F, twdi2_R, twdi2_R, twdi2_F, twdi2_F))
	port map( Input => i2, Output => connect(2));

   -- Netlist
   U4 : TLU
	generic map(
		N => 3,
		TruthTable => "01010111",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 i2 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi2_q_R, tpdi2_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XXX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Input(2) => connect(2), 
		Output => q);


end FTGS;

configuration CFG_oa22_x4_FTGS of oa22_x4 is
   for FTGS
   end for;
end CFG_oa22_x4_FTGS;


----- CELL on12_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity on12_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.111 ns;
      tpdi0_q_F                      :	Time := 0.234 ns;
      tpdi1_q_R                      :	Time := 0.314 ns;
      tpdi1_q_F                      :	Time := 0.291 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end on12_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of on12_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1101",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_on12_x1_FTGS of on12_x1 is
   for FTGS
   end for;
end CFG_on12_x1_FTGS;


----- CELL on12_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity on12_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.474 ns;
      tpdi0_q_F                      :	Time := 0.499 ns;
      tpdi1_q_R                      :	Time := 0.491 ns;
      tpdi1_q_F                      :	Time := 0.394 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end on12_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of on12_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "1101",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_on12_x4_FTGS of on12_x4 is
   for FTGS
   end for;
end CFG_on12_x4_FTGS;


----- CELL one_x0 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity one_x0 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False);

   port(
      q                              :	out   STD_LOGIC := '1');
end one_x0;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of one_x0 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes



begin

   -- Netlist
   q <= '1';

end FTGS;

configuration CFG_one_x0_FTGS of one_x0 is
   for FTGS
   end for;
end CFG_one_x0_FTGS;


----- CELL sff1_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity sff1_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdck_q_R                      :	Time := 0.500 ns;
      tpdck_q_F                      :	Time := 0.500 ns;
      tsui_ck                        :	Time := 0.585 ns;
      thck_i                         :	Time := 0.000 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns;
      twdck_R                        :	Time := 0.000 ns;
      twdck_F                        :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      ck                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end sff1_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of sff1_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of thck_i: constant is 
	"U3/constraint_param(1).Check_time";
   attribute PROPAGATE_VALUE of tsui_ck: constant is 
	"U3/constraint_param(0).Check_time";
   attribute PROPAGATE_VALUE of tpdck_q_F: constant is 
	"U3/delay_param(0)(TRAN_10), " &
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdck_q_R: constant is 
	"U3/delay_param(0)(TRAN_01), " &
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdck_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdck_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdck_R, twdck_F, twdck_R, twdck_R, twdck_F, twdck_F))
	port map( Input => ck, Output => connect(1));

   -- Netlist
   U3 : SEQGEN
	generic map(
		N_enable => 0,
		N_clock => 1,
		N_clear => 0,
		N_preset => 0,
		N_data => 1,
		N_cond_signal => 0,
		lut_enable => "",
		lut_clock => "01",
		lut_clear => "",
		lut_preset => "",
		lut_data => "01",
		TT_size_data => nil_integer_vector,
		Node_Index_data => nil_integer_vector,
		lut_next => "NN01NN01",
		pin_names => "ck i q",
		delay_param =>
			((tpdck_q_R, tpdck_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdck_q_R, tpdck_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		constraint_param => 
			((1, 0, setup_rising_ff, tsui_ck),
			 (0, 1, hold_rising_ff, thck_i)),
		InMapZ => "XX",
		Q_feedback => FALSE,
		Enable_feedback => FALSE,
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		Constraint_mesg => Timing_mesg,
		Constraint_xgen => Timing_xgen,
		strn => strn_X01)
	port map(	Input(0) => connect(1),
			Input(1) => connect(0),
			Output => q);


end FTGS;

configuration CFG_sff1_x4_FTGS of sff1_x4 is
   for FTGS
   end for;
end CFG_sff1_x4_FTGS;


----- CELL sff2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity sff2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdck_q_R                      :	Time := 0.500 ns;
      tpdck_q_F                      :	Time := 0.500 ns;
      tsui0_ck                       :	Time := 0.764 ns;
      thck_i0                        :	Time := 0.000 ns;
      tsui1_ck                       :	Time := 0.764 ns;
      thck_i1                        :	Time := 0.000 ns;
      tsucmd_ck                      :	Time := 0.833 ns;
      thck_cmd                       :	Time := 0.000 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns;
      twdck_R                        :	Time := 0.000 ns;
      twdck_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      ck                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end sff2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of sff2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of thck_cmd: constant is 
	"U5/constraint_param(5).Check_time";
   attribute PROPAGATE_VALUE of tsucmd_ck: constant is 
	"U5/constraint_param(4).Check_time";
   attribute PROPAGATE_VALUE of thck_i1: constant is 
	"U5/constraint_param(3).Check_time";
   attribute PROPAGATE_VALUE of tsui1_ck: constant is 
	"U5/constraint_param(2).Check_time";
   attribute PROPAGATE_VALUE of thck_i0: constant is 
	"U5/constraint_param(1).Check_time";
   attribute PROPAGATE_VALUE of tsui0_ck: constant is 
	"U5/constraint_param(0).Check_time";
   attribute PROPAGATE_VALUE of tpdck_q_F: constant is 
	"U5/delay_param(0)(TRAN_10), " &
	"U5/delay_param(1)(TRAN_10), U5/delay_param(2)(TRAN_10), " &
	"U5/delay_param(3)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdck_q_R: constant is 
	"U5/delay_param(0)(TRAN_01), " &
	"U5/delay_param(1)(TRAN_01), U5/delay_param(2)(TRAN_01), " &
	"U5/delay_param(3)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdck_F: constant is 
	"U4/delay(TRAN_10), " &
	"U4/delay(TRAN_1Z), U4/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdck_R: constant is 
	"U4/delay(TRAN_01), " &
	"U4/delay(TRAN_0Z), U4/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U3/delay(TRAN_10), " &
	"U3/delay(TRAN_1Z), U3/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U3/delay(TRAN_01), " &
	"U3/delay(TRAN_0Z), U3/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   U3 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(2));

   U4 : WIREBUF
	generic map(delay => (twdck_R, twdck_F, twdck_R, twdck_R, twdck_F, twdck_F))
	port map( Input => ck, Output => connect(3));

   -- Netlist
   U5 : SEQGEN
	generic map(
		N_enable => 0,
		N_clock => 1,
		N_clear => 0,
		N_preset => 0,
		N_data => 3,
		N_cond_signal => 0,
		lut_enable => "",
		lut_clock => "01",
		lut_clear => "",
		lut_preset => "",
		lut_data => "00011011",
		TT_size_data => nil_integer_vector,
		Node_Index_data => nil_integer_vector,
		lut_next => "NN01NN01",
		pin_names => "ck i0 i1 cmd q",
		delay_param =>
			((tpdck_q_R, tpdck_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdck_q_R, tpdck_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdck_q_R, tpdck_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdck_q_R, tpdck_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		constraint_param => 
			((1, 0, setup_rising_ff, tsui0_ck),
			 (0, 1, hold_rising_ff, thck_i0),
			 (2, 0, setup_rising_ff, tsui1_ck),
			 (0, 2, hold_rising_ff, thck_i1),
			 (3, 0, setup_rising_ff, tsucmd_ck),
			 (0, 3, hold_rising_ff, thck_cmd)),
		InMapZ => "XXXX",
		Q_feedback => FALSE,
		Enable_feedback => FALSE,
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		Constraint_mesg => Timing_mesg,
		Constraint_xgen => Timing_xgen,
		strn => strn_X01)
	port map(	Input(0) => connect(3),
			Input(1) => connect(0),
			Input(2) => connect(1),
			Input(3) => connect(2),
			Output => q);


end FTGS;

configuration CFG_sff2_x4_FTGS of sff2_x4 is
   for FTGS
   end for;
end CFG_sff2_x4_FTGS;


----- CELL ts_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity ts_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_q_R                     :	Time := 0.492 ns;
      tpdcmd_q_F                     :	Time := 0.409 ns;
      tpdcmd_q_LZ                    :	Time := 0.492 ns;
      tpdcmd_q_HZ                    :	Time := 0.409 ns;
      tpdi_q_R                       :	Time := 0.475 ns;
      tpdi_q_F                       :	Time := 0.444 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end ts_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of ts_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdcmd_q_HZ: constant is 
	"U3/delay_param(1)(TRAN_1Z)";
   attribute PROPAGATE_VALUE of tpdcmd_q_LZ: constant is 
	"U3/delay_param(1)(TRAN_0Z)";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is 
	"U3/delay_param(1)(TRAN_10), " &
	"U3/delay_param(1)(TRAN_Z0)";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is 
	"U3/delay_param(1)(TRAN_01), " &
	"U3/delay_param(1)(TRAN_Z1)";
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(1));

   U2 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "Z0Z1",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i cmd q",
		delay_param =>
			((tpdi_q_R, tpdi_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd_q_R, tpdcmd_q_F, tpdcmd_q_LZ, tpdcmd_q_R, tpdcmd_q_HZ, tpdcmd_q_F)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(1), 
		Input(1) => connect(0), 
		Output => q);


end FTGS;

configuration CFG_ts_x4_FTGS of ts_x4 is
   for FTGS
   end for;
end CFG_ts_x4_FTGS;


----- CELL ts_x8 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity ts_x8 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdcmd_q_R                     :	Time := 0.626 ns;
      tpdcmd_q_F                     :	Time := 0.466 ns;
      tpdcmd_q_LZ                    :	Time := 0.626 ns;
      tpdcmd_q_HZ                    :	Time := 0.466 ns;
      tpdi_q_R                       :	Time := 0.613 ns;
      tpdi_q_F                       :	Time := 0.569 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns;
      twdcmd_R                       :	Time := 0.000 ns;
      twdcmd_F                       :	Time := 0.000 ns);

   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end ts_x8;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of ts_x8 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdcmd_q_HZ: constant is 
	"U3/delay_param(1)(TRAN_1Z)";
   attribute PROPAGATE_VALUE of tpdcmd_q_LZ: constant is 
	"U3/delay_param(1)(TRAN_0Z)";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is 
	"U3/delay_param(1)(TRAN_10), " &
	"U3/delay_param(1)(TRAN_Z0)";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is 
	"U3/delay_param(1)(TRAN_01), " &
	"U3/delay_param(1)(TRAN_Z1)";
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi_R, twdi_F, twdi_R, twdi_R, twdi_F, twdi_F))
	port map( Input => i, Output => connect(1));

   U2 : WIREBUF
	generic map(delay => (twdcmd_R, twdcmd_F, twdcmd_R, twdcmd_R, twdcmd_F, twdcmd_F))
	port map( Input => cmd, Output => connect(0));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "Z0Z1",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i cmd q",
		delay_param =>
			((tpdi_q_R, tpdi_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdcmd_q_R, tpdcmd_q_F, tpdcmd_q_LZ, tpdcmd_q_R, tpdcmd_q_HZ, tpdcmd_q_F)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(1), 
		Input(1) => connect(0), 
		Output => q);


end FTGS;

configuration CFG_ts_x8_FTGS of ts_x8 is
   for FTGS
   end for;
end CFG_ts_x8_FTGS;


----- CELL xr2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity xr2_x1 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.292 ns;
      tpdi0_q_F                      :	Time := 0.293 ns;
      tpdi1_q_R                      :	Time := 0.377 ns;
      tpdi1_q_F                      :	Time := 0.261 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end xr2_x1;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of xr2_x1 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_xr2_x1_FTGS of xr2_x1 is
   for FTGS
   end for;
end CFG_xr2_x1_FTGS;


----- CELL xr2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity xr2_x4 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False;
      tpdi0_q_R                      :	Time := 0.521 ns;
      tpdi0_q_F                      :	Time := 0.560 ns;
      tpdi1_q_R                      :	Time := 0.541 ns;
      tpdi1_q_F                      :	Time := 0.657 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end xr2_x4;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of xr2_x4 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is 
	"U3/delay_param(1)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is 
	"U3/delay_param(1)(TRAN_01)";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is 
	"U3/delay_param(0)(TRAN_10)";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is 
	"U3/delay_param(0)(TRAN_01)";
   attribute PROPAGATE_VALUE of twdi1_F: constant is 
	"U2/delay(TRAN_10), " &
	"U2/delay(TRAN_1Z), U2/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi1_R: constant is 
	"U2/delay(TRAN_01), " &
	"U2/delay(TRAN_0Z), U2/delay(TRAN_Z1)";
   attribute PROPAGATE_VALUE of twdi0_F: constant is 
	"U1/delay(TRAN_10), " &
	"U1/delay(TRAN_1Z), U1/delay(TRAN_Z0)";
   attribute PROPAGATE_VALUE of twdi0_R: constant is 
	"U1/delay(TRAN_01), " &
	"U1/delay(TRAN_0Z), U1/delay(TRAN_Z1)";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U'); 


begin

   -- Extrinsic delay buffers
   U1 : WIREBUF
	generic map(delay => (twdi0_R, twdi0_F, twdi0_R, twdi0_R, twdi0_F, twdi0_F))
	port map( Input => i0, Output => connect(0));

   U2 : WIREBUF
	generic map(delay => (twdi1_R, twdi1_F, twdi1_R, twdi1_R, twdi1_F, twdi1_F))
	port map( Input => i1, Output => connect(1));

   -- Netlist
   U3 : TLU
	generic map(
		N => 2,
		TruthTable => "0110",
		TT_size => nil_integer_vector,
		Node_index => nil_integer_vector,
		pin_names => "i0 i1 q",
		delay_param =>
			((tpdi0_q_R, tpdi0_q_F, 0 ns, 0 ns, 0 ns, 0 ns),
			 (tpdi1_q_R, tpdi1_q_F, 0 ns, 0 ns, 0 ns, 0 ns)),
		InMapZ => "XX",
		OutMapZ => 'Z',
		PulseHandling => PH_GLITCH,
		Timing_mesg => Timing_mesg,
		Timing_xgen => Timing_xgen,
		strn => strn_X01)
	port map(
		Input(0) => connect(0), 
		Input(1) => connect(1), 
		Output => q);


end FTGS;

configuration CFG_xr2_x4_FTGS of xr2_x4 is
   for FTGS
   end for;
end CFG_xr2_x4_FTGS;


----- CELL zero_x0 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library SYNOPSYS;
use SYNOPSYS.attributes.REAL_NAME;

-- entity declaration --
entity zero_x0 is
   generic(
      Timing_mesg: Boolean := True;
      Timing_xgen: Boolean := False);

   port(
      nq                             :	out   STD_LOGIC := '0');
end zero_x0;

-- architecture body --
library IEEE_ASIC;
use IEEE.STD_LOGIC_MISC.all;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

library IEEE;
use IEEE.GS_TYPES.all;
library GSCOMP;
use GSCOMP.GS_COMPONENTS.all;

architecture FTGS of zero_x0 is
   attribute PRIVATE of FTGS : architecture is TRUE;
   attribute ASIC_CELL of FTGS : architecture is TRUE;

   -- Backannotation attributes



begin

   -- Netlist
   nq <= '0';

end FTGS;

configuration CFG_zero_x0_FTGS of zero_x0 is
   for FTGS
   end for;
end CFG_zero_x0_FTGS;


---- end of library ----
