
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler 1999.10
-- FILENAME     :    sxlib_FTSM.vhd
-- FILE CONTENTS:    Entity, Structural Architecture(FTSM),
--                   and Configuration
-- DATE CREATED :    Mon May  7 10:19:50 2001
-- 
-- LIBRARY      :    sxlib
-- DATE ENTERED :    Thu Dec 21 11:24:55 MET 2000
-- REVISION     :    1.200000
-- TECHNOLOGY   :    cmos
-- TIME SCALE   :    1 ns
-- LOGIC SYSTEM :    IEEE-1164
-- NOTES        :    FTSM, Timing_mesg(TRUE)
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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of a2_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : AND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => q);


end FTSM;

configuration CFG_a2_x2_FTSM of a2_x2 is
   for FTSM
   end for;
end CFG_a2_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of a2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : AND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => q);


end FTSM;

configuration CFG_a2_x4_FTSM of a2_x4 is
   for FTSM
   end for;
end CFG_a2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of a3_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component AND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : AND3MAC
	port map( I0 => prop_q(1), I1 => prop_q(2), I2 => prop_q(0), Y => 
         q);


end FTSM;

configuration CFG_a3_x2_FTSM of a3_x2 is
   for FTSM
   end for;
end CFG_a3_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of a3_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component AND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : AND3MAC
	port map( I0 => prop_q(1), I1 => prop_q(2), I2 => prop_q(0), Y => 
         q);


end FTSM;

configuration CFG_a3_x4_FTSM of a3_x4 is
   for FTSM
   end for;
end CFG_a3_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of a4_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component AND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : AND4MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), I2 => prop_q(2), I3 => 
         prop_q(3), Y => q);


end FTSM;

configuration CFG_a4_x2_FTSM of a4_x2 is
   for FTSM
   end for;
end CFG_a4_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of a4_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component AND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : AND4MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), I2 => prop_q(2), I3 => 
         prop_q(3), Y => q);


end FTSM;

configuration CFG_a4_x4_FTSM of a4_x4 is
   for FTSM
   end for;
end CFG_a4_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of an12_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal n1 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_F, tHL => tpdi0_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : AND2MAC
	port map( I0 => prop_q(1), I1 => n1, Y => q);

   U6 : INVMAC
	port map( I0 => prop_q(0), Y => n1);


end FTSM;

configuration CFG_an12_x1_FTSM of an12_x1 is
   for FTSM
   end for;
end CFG_an12_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of an12_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal n1 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_F, tHL => tpdi0_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : AND2MAC
	port map( I0 => prop_q(1), I1 => n1, Y => q);

   U6 : INVMAC
	port map( I0 => prop_q(0), Y => n1);


end FTSM;

configuration CFG_an12_x4_FTSM of an12_x4 is
   for FTSM
   end for;
end CFG_an12_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of ao2o22_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U10 : OR2MAC
	port map( I0 => prop_q(3), I1 => prop_q(2), Y => n2);

   U11 : OR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => n1);


end FTSM;

configuration CFG_ao2o22_x2_FTSM of ao2o22_x2 is
   for FTSM
   end for;
end CFG_ao2o22_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of ao2o22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U10 : OR2MAC
	port map( I0 => prop_q(3), I1 => prop_q(2), Y => n2);

   U11 : OR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => n1);


end FTSM;

configuration CFG_ao2o22_x4_FTSM of ao2o22_x4 is
   for FTSM
   end for;
end CFG_ao2o22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of ao22_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : AND2MAC
	port map( I0 => prop_q(2), I1 => n1, Y => q);

   U8 : OR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => n1);


end FTSM;

configuration CFG_ao22_x2_FTSM of ao22_x2 is
   for FTSM
   end for;
end CFG_ao22_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of ao22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : AND2MAC
	port map( I0 => prop_q(2), I1 => n1, Y => q);

   U8 : OR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => n1);


end FTSM;

configuration CFG_ao22_x4_FTSM of ao22_x4 is
   for FTSM
   end for;
end CFG_ao22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of buf_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Concurrent assignments
   U2 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_q_R, tHL => tpdi_q_F)
	port map( Input => connect(0), Output => q);


end FTSM;

configuration CFG_buf_x2_FTSM of buf_x2 is
   for FTSM
   end for;
end CFG_buf_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of buf_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Concurrent assignments
   U2 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_q_R, tHL => tpdi_q_F)
	port map( Input => connect(0), Output => q);


end FTSM;

configuration CFG_buf_x4_FTSM of buf_x4 is
   for FTSM
   end for;
end CFG_buf_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of buf_x8 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Concurrent assignments
   U2 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_q_R, tHL => tpdi_q_F)
	port map( Input => connect(0), Output => q);


end FTSM;

configuration CFG_buf_x8_FTSM of buf_x8 is
   for FTSM
   end for;
end CFG_buf_x8_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of inv_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is "U2/U1/tHL";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is "U2/U1/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : INVMAC
	generic map( tpdY_R => tpdi_nq_R, tpdY_F => tpdi_nq_F )
	port map( I0 => connect(0), Y => nq);


end FTSM;

configuration CFG_inv_x1_FTSM of inv_x1 is
   for FTSM
   end for;
end CFG_inv_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of inv_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is "U2/U1/tHL";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is "U2/U1/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : INVMAC
	generic map( tpdY_R => tpdi_nq_R, tpdY_F => tpdi_nq_F )
	port map( I0 => connect(0), Y => nq);


end FTSM;

configuration CFG_inv_x2_FTSM of inv_x2 is
   for FTSM
   end for;
end CFG_inv_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of inv_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is "U2/U1/tHL";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is "U2/U1/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : INVMAC
	generic map( tpdY_R => tpdi_nq_R, tpdY_F => tpdi_nq_F )
	port map( I0 => connect(0), Y => nq);


end FTSM;

configuration CFG_inv_x4_FTSM of inv_x4 is
   for FTSM
   end for;
end CFG_inv_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of inv_x8 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is "U2/U1/tHL";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is "U2/U1/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 0) := (others => 'U');

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   -- Netlist
   U2 : INVMAC
	generic map( tpdY_R => tpdi_nq_R, tpdY_F => tpdi_nq_F )
	port map( I0 => connect(0), Y => nq);


end FTSM;

configuration CFG_inv_x8_FTSM of inv_x8 is
   for FTSM
   end for;
end CFG_inv_x8_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of mx2_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_q_R, tHL => tpdcmd_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : MUX2MAC
	port map( I0 => prop_q(1), I1 => prop_q(2), S0 => prop_q(0), Y => 
         q);


end FTSM;

configuration CFG_mx2_x2_FTSM of mx2_x2 is
   for FTSM
   end for;
end CFG_mx2_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of mx2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_q_R, tHL => tpdcmd_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : MUX2MAC
	port map( I0 => prop_q(1), I1 => prop_q(2), S0 => prop_q(0), Y => 
         q);


end FTSM;

configuration CFG_mx2_x4_FTSM of mx2_x4 is
   for FTSM
   end for;
end CFG_mx2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of mx3_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdcmd1_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdcmd1_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdcmd0_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdcmd0_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd0_R, tHL => twdcmd0_F)
	port map( Input => cmd0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd1_R, tHL => twdcmd1_F)
	port map( Input => cmd1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd0_q_R, tHL => tpdcmd0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd1_q_R, tHL => tpdcmd1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   -- Netlist
   U11 : MUX2MAC
	port map( I0 => prop_q(4), I1 => prop_q(3), S0 => prop_q(1), Y => 
         n1);

   U12 : MUX2MAC
	port map( I0 => prop_q(2), I1 => n1, S0 => prop_q(0), Y => q);


end FTSM;

configuration CFG_mx3_x2_FTSM of mx3_x2 is
   for FTSM
   end for;
end CFG_mx3_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of mx3_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdcmd1_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdcmd1_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdcmd0_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdcmd0_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd0_R, tHL => twdcmd0_F)
	port map( Input => cmd0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd1_R, tHL => twdcmd1_F)
	port map( Input => cmd1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd0_q_R, tHL => tpdcmd0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd1_q_R, tHL => tpdcmd1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   -- Netlist
   U11 : MUX2MAC
	port map( I0 => prop_q(4), I1 => prop_q(3), S0 => prop_q(1), Y => 
         n1);

   U12 : MUX2MAC
	port map( I0 => prop_q(2), I1 => n1, S0 => prop_q(0), Y => q);


end FTSM;

configuration CFG_mx3_x4_FTSM of mx3_x4 is
   for FTSM
   end for;
end CFG_mx3_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of na2_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => nq);


end FTSM;

configuration CFG_na2_x1_FTSM of na2_x1 is
   for FTSM
   end for;
end CFG_na2_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of na2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => nq);


end FTSM;

configuration CFG_na2_x4_FTSM of na2_x4 is
   for FTSM
   end for;
end CFG_na2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of na3_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NAND3MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(2), I2 => prop_nq(0), Y => 
         nq);


end FTSM;

configuration CFG_na3_x1_FTSM of na3_x1 is
   for FTSM
   end for;
end CFG_na3_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of na3_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NAND3MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(2), I2 => prop_nq(0), Y => 
         nq);


end FTSM;

configuration CFG_na3_x4_FTSM of na3_x4 is
   for FTSM
   end for;
end CFG_na3_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of na4_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component NAND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : NAND4MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), I2 => prop_nq(2), I3 => 
         prop_nq(3), Y => nq);


end FTSM;

configuration CFG_na4_x1_FTSM of na4_x1 is
   for FTSM
   end for;
end CFG_na4_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of na4_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component NAND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : NAND4MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), I2 => prop_nq(2), I3 => 
         prop_nq(3), Y => nq);


end FTSM;

configuration CFG_na4_x4_FTSM of na4_x4 is
   for FTSM
   end for;
end CFG_na4_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nao2o22_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U10 : OR2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n2);

   U11 : OR2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_nao2o22_x1_FTSM of nao2o22_x1 is
   for FTSM
   end for;
end CFG_nao2o22_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nao2o22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U10 : OR2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n2);

   U11 : OR2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_nao2o22_x4_FTSM of nao2o22_x4 is
   for FTSM
   end for;
end CFG_nao2o22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nao22_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => n1, Y => nq);

   U8 : OR2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_nao22_x1_FTSM of nao22_x1 is
   for FTSM
   end for;
end CFG_nao22_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nao22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => n1, Y => nq);

   U8 : OR2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_nao22_x4_FTSM of nao22_x4 is
   for FTSM
   end for;
end CFG_nao22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nmx2_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_nq_F, tHL => tpdcmd_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : MUX2MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(2), S0 => prop_nq(0), Y => 
         n1);

   U8 : INVMAC
	port map( I0 => n1, Y => nq);


end FTSM;

configuration CFG_nmx2_x1_FTSM of nmx2_x1 is
   for FTSM
   end for;
end CFG_nmx2_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nmx2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_nq_F, tHL => tpdcmd_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : MUX2MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(2), S0 => prop_nq(0), Y => 
         n1);

   U8 : INVMAC
	port map( I0 => n1, Y => nq);


end FTSM;

configuration CFG_nmx2_x4_FTSM of nmx2_x4 is
   for FTSM
   end for;
end CFG_nmx2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nmx3_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd0_R, tHL => twdcmd0_F)
	port map( Input => cmd0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd1_R, tHL => twdcmd1_F)
	port map( Input => cmd1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd0_nq_F, tHL => tpdcmd0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd1_nq_F, tHL => tpdcmd1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   -- Netlist
   U11 : MUX2MAC
	port map( I0 => prop_nq(4), I1 => prop_nq(3), S0 => prop_nq(1), Y => 
         n1);

   U12 : MUX2MAC
	port map( I0 => prop_nq(2), I1 => n1, S0 => prop_nq(0), Y => n2);

   U13 : INVMAC
	port map( I0 => n2, Y => nq);


end FTSM;

configuration CFG_nmx3_x1_FTSM of nmx3_x1 is
   for FTSM
   end for;
end CFG_nmx3_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nmx3_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdcmd1_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdcmd0_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdcmd1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdcmd0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdcmd0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd0_R, tHL => twdcmd0_F)
	port map( Input => cmd0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd1_R, tHL => twdcmd1_F)
	port map( Input => cmd1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd0_nq_F, tHL => tpdcmd0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd1_nq_F, tHL => tpdcmd1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   -- Netlist
   U11 : MUX2MAC
	port map( I0 => prop_nq(4), I1 => prop_nq(3), S0 => prop_nq(1), Y => 
         n1);

   U12 : MUX2MAC
	port map( I0 => prop_nq(2), I1 => n1, S0 => prop_nq(0), Y => n2);

   U13 : INVMAC
	port map( I0 => n2, Y => nq);


end FTSM;

configuration CFG_nmx3_x4_FTSM of nmx3_x4 is
   for FTSM
   end for;
end CFG_nmx3_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of no2_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component NOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : NOR2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => nq);


end FTSM;

configuration CFG_no2_x1_FTSM of no2_x1 is
   for FTSM
   end for;
end CFG_no2_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of no2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component NOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : NOR2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => nq);


end FTSM;

configuration CFG_no2_x4_FTSM of no2_x4 is
   for FTSM
   end for;
end CFG_no2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of no3_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component NOR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NOR3MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(0), I2 => prop_nq(1), Y => 
         nq);


end FTSM;

configuration CFG_no3_x1_FTSM of no3_x1 is
   for FTSM
   end for;
end CFG_no3_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of no3_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component NOR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NOR3MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(0), I2 => prop_nq(1), Y => 
         nq);


end FTSM;

configuration CFG_no3_x4_FTSM of no3_x4 is
   for FTSM
   end for;
end CFG_no3_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of no4_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component NOR4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : NOR4MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), I2 => prop_nq(2), I3 => 
         prop_nq(3), Y => nq);


end FTSM;

configuration CFG_no4_x1_FTSM of no4_x1 is
   for FTSM
   end for;
end CFG_no4_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of no4_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component NOR4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : NOR4MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), I2 => prop_nq(2), I3 => 
         prop_nq(3), Y => nq);


end FTSM;

configuration CFG_no4_x4_FTSM of no4_x4 is
   for FTSM
   end for;
end CFG_no4_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2a2a2a24_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi7_nq_F: constant is "U16/tLH";
   attribute PROPAGATE_VALUE of tpdi7_nq_R: constant is "U16/tHL";
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is "U15/tLH";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is "U15/tHL";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of twdi7_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi7_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal n1, n2, n3, n4 : STD_LOGIC;

   component AND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   U8 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi7_R, tHL => twdi7_F)
	port map( Input => i7, Output => connect(7));

   -- Intrinsic delay buffers
   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_nq_F, tHL => tpdi5_nq_R)
	port map( Input => connect(5), Output => prop_nq(5));

   U15 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_nq_F, tHL => tpdi6_nq_R)
	port map( Input => connect(6), Output => prop_nq(6));

   U16 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi7_nq_F, tHL => tpdi7_nq_R)
	port map( Input => connect(7), Output => prop_nq(7));

   -- Netlist
   U17 : AND4MAC
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => nq);

   U18 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n4);

   U19 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n3);

   U20 : NAND2MAC
	port map( I0 => prop_nq(4), I1 => prop_nq(5), Y => n2);

   U21 : NAND2MAC
	port map( I0 => prop_nq(6), I1 => prop_nq(7), Y => n1);


end FTSM;

configuration CFG_noa2a2a2a24_x1_FTSM of noa2a2a2a24_x1 is
   for FTSM
   end for;
end CFG_noa2a2a2a24_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2a2a2a24_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi7_nq_F: constant is "U16/tLH";
   attribute PROPAGATE_VALUE of tpdi7_nq_R: constant is "U16/tHL";
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is "U15/tLH";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is "U15/tHL";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of twdi7_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi7_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal n1, n2, n3, n4 : STD_LOGIC;

   component AND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   U8 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi7_R, tHL => twdi7_F)
	port map( Input => i7, Output => connect(7));

   -- Intrinsic delay buffers
   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_nq_F, tHL => tpdi5_nq_R)
	port map( Input => connect(5), Output => prop_nq(5));

   U15 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_nq_F, tHL => tpdi6_nq_R)
	port map( Input => connect(6), Output => prop_nq(6));

   U16 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi7_nq_F, tHL => tpdi7_nq_R)
	port map( Input => connect(7), Output => prop_nq(7));

   -- Netlist
   U17 : AND4MAC
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => nq);

   U18 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n4);

   U19 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n3);

   U20 : NAND2MAC
	port map( I0 => prop_nq(4), I1 => prop_nq(5), Y => n2);

   U21 : NAND2MAC
	port map( I0 => prop_nq(6), I1 => prop_nq(7), Y => n1);


end FTSM;

configuration CFG_noa2a2a2a24_x4_FTSM of noa2a2a2a24_x4 is
   for FTSM
   end for;
end CFG_noa2a2a2a24_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2a2a23_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component AND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   -- Intrinsic delay buffers
   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_nq_F, tHL => tpdi5_nq_R)
	port map( Input => connect(5), Output => prop_nq(5));

   -- Netlist
   U13 : AND3MAC
	port map( I0 => n1, I1 => n2, I2 => n3, Y => nq);

   U14 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n2);

   U15 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);

   U16 : NAND2MAC
	port map( I0 => prop_nq(4), I1 => prop_nq(5), Y => n3);


end FTSM;

configuration CFG_noa2a2a23_x1_FTSM of noa2a2a23_x1 is
   for FTSM
   end for;
end CFG_noa2a2a23_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2a2a23_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component AND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   -- Intrinsic delay buffers
   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_nq_F, tHL => tpdi5_nq_R)
	port map( Input => connect(5), Output => prop_nq(5));

   -- Netlist
   U13 : AND3MAC
	port map( I0 => n1, I1 => n2, I2 => n3, Y => nq);

   U14 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n2);

   U15 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);

   U16 : NAND2MAC
	port map( I0 => prop_nq(4), I1 => prop_nq(5), Y => n3);


end FTSM;

configuration CFG_noa2a2a23_x4_FTSM of noa2a2a23_x4 is
   for FTSM
   end for;
end CFG_noa2a2a23_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2a22_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U10 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n2);

   U11 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_noa2a22_x1_FTSM of noa2a22_x1 is
   for FTSM
   end for;
end CFG_noa2a22_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2a22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   -- Netlist
   U9 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U10 : NAND2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n2);

   U11 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_noa2a22_x4_FTSM of noa2a22_x4 is
   for FTSM
   end for;
end CFG_noa2a22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2ao222_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   -- Netlist
   U11 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U12 : OR2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n3);

   U13 : NAND2MAC
	port map( I0 => prop_nq(4), I1 => n3, Y => n2);

   U14 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_noa2ao222_x1_FTSM of noa2ao222_x1 is
   for FTSM
   end for;
end CFG_noa2ao222_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa2ao222_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   -- Netlist
   U11 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U12 : OR2MAC
	port map( I0 => prop_nq(2), I1 => prop_nq(3), Y => n3);

   U13 : NAND2MAC
	port map( I0 => prop_nq(4), I1 => n3, Y => n2);

   U14 : NAND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_noa2ao222_x4_FTSM of noa2ao222_x4 is
   for FTSM
   end for;
end CFG_noa2ao222_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa3ao322_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   -- Intrinsic delay buffers
   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_nq_F, tHL => tpdi5_nq_R)
	port map( Input => connect(5), Output => prop_nq(5));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_nq_F, tHL => tpdi6_nq_R)
	port map( Input => connect(6), Output => prop_nq(6));

   -- Netlist
   U15 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U16 : NAND3MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(2), I2 => prop_nq(0), Y => 
         n2);

   U17 : OR3MAC
	port map( I0 => prop_nq(3), I1 => prop_nq(4), I2 => prop_nq(5), Y => 
         n3);

   U18 : NAND2MAC
	port map( I0 => prop_nq(6), I1 => n3, Y => n1);


end FTSM;

configuration CFG_noa3ao322_x1_FTSM of noa3ao322_x1 is
   for FTSM
   end for;
end CFG_noa3ao322_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa3ao322_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi6_nq_F: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi6_nq_R: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi5_nq_F: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi5_nq_R: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi4_nq_F: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi4_nq_R: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi3_nq_F: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi3_nq_R: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   -- Intrinsic delay buffers
   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_nq_F, tHL => tpdi3_nq_R)
	port map( Input => connect(3), Output => prop_nq(3));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_nq_F, tHL => tpdi4_nq_R)
	port map( Input => connect(4), Output => prop_nq(4));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_nq_F, tHL => tpdi5_nq_R)
	port map( Input => connect(5), Output => prop_nq(5));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_nq_F, tHL => tpdi6_nq_R)
	port map( Input => connect(6), Output => prop_nq(6));

   -- Netlist
   U15 : AND2MAC
	port map( I0 => n1, I1 => n2, Y => nq);

   U16 : NAND3MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(2), I2 => prop_nq(0), Y => 
         n2);

   U17 : OR3MAC
	port map( I0 => prop_nq(3), I1 => prop_nq(4), I2 => prop_nq(5), Y => 
         n3);

   U18 : NAND2MAC
	port map( I0 => prop_nq(6), I1 => n3, Y => n1);


end FTSM;

configuration CFG_noa3ao322_x4_FTSM of noa3ao322_x4 is
   for FTSM
   end for;
end CFG_noa3ao322_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa22_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component NOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NOR2MAC
	port map( I0 => prop_nq(2), I1 => n1, Y => nq);

   U8 : AND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_noa22_x1_FTSM of noa22_x1 is
   for FTSM
   end for;
end CFG_noa22_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of noa22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_nq_F: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_nq_R: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component NOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_nq_F, tHL => tpdi2_nq_R)
	port map( Input => connect(2), Output => prop_nq(2));

   -- Netlist
   U7 : NOR2MAC
	port map( I0 => prop_nq(2), I1 => n1, Y => nq);

   U8 : AND2MAC
	port map( I0 => prop_nq(0), I1 => prop_nq(1), Y => n1);


end FTSM;

configuration CFG_noa22_x4_FTSM of noa22_x4 is
   for FTSM
   end for;
end CFG_noa22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nts_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is "U3/tHL, U3/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_nq_HZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_LZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component INV3SHEMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         OE                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(1));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_nq_R, tHL => tpdcmd_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_nq_F, tHL => tpdi_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : INV3SHEMAC
	port map( I0 => prop_nq(1), OE => prop_nq(0), Y => nq);


end FTSM;

configuration CFG_nts_x1_FTSM of nts_x1 is
   for FTSM
   end for;
end CFG_nts_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nts_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_R: constant is "U3/tHL, U3/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_nq_HZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_LZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_nq_F: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component INV3SHEMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         OE                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(1));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_nq_R, tHL => tpdcmd_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_nq_F, tHL => tpdi_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : INV3SHEMAC
	port map( I0 => prop_nq(1), OE => prop_nq(0), Y => nq);


end FTSM;

configuration CFG_nts_x2_FTSM of nts_x2 is
   for FTSM
   end for;
end CFG_nts_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nxr2_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component NXOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : NXOR2MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(0), Y => nq);


end FTSM;

configuration CFG_nxr2_x1_FTSM of nxr2_x1 is
   for FTSM
   end for;
end CFG_nxr2_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of nxr2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_nq_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_nq_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_nq_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_nq_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_nq : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component NXOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_nq_F, tHL => tpdi0_nq_R)
	port map( Input => connect(0), Output => prop_nq(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_nq_F, tHL => tpdi1_nq_R)
	port map( Input => connect(1), Output => prop_nq(1));

   -- Netlist
   U5 : NXOR2MAC
	port map( I0 => prop_nq(1), I1 => prop_nq(0), Y => nq);


end FTSM;

configuration CFG_nxr2_x4_FTSM of nxr2_x4 is
   for FTSM
   end for;
end CFG_nxr2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of o2_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : OR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => q);


end FTSM;

configuration CFG_o2_x2_FTSM of o2_x2 is
   for FTSM
   end for;
end CFG_o2_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of o2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : OR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => q);


end FTSM;

configuration CFG_o2_x4_FTSM of o2_x4 is
   for FTSM
   end for;
end CFG_o2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of o3_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component OR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : OR3MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), I2 => prop_q(2), Y => 
         q);


end FTSM;

configuration CFG_o3_x2_FTSM of o3_x2 is
   for FTSM
   end for;
end CFG_o3_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of o3_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');

   component OR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : OR3MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), I2 => prop_q(2), Y => 
         q);


end FTSM;

configuration CFG_o3_x4_FTSM of o3_x4 is
   for FTSM
   end for;
end CFG_o3_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of o4_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component OR4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : OR4MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), I2 => prop_q(0), I3 => 
         prop_q(1), Y => q);


end FTSM;

configuration CFG_o4_x2_FTSM of o4_x2 is
   for FTSM
   end for;
end CFG_o4_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of o4_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');

   component OR4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : OR4MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), I2 => prop_q(0), I3 => 
         prop_q(1), Y => q);


end FTSM;

configuration CFG_o4_x4_FTSM of o4_x4 is
   for FTSM
   end for;
end CFG_o4_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2a2a2a24_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi7_q_R: constant is "U16/tLH";
   attribute PROPAGATE_VALUE of tpdi7_q_F: constant is "U16/tHL";
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is "U15/tLH";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is "U15/tHL";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of twdi7_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi7_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal n1, n2, n3, n4 : STD_LOGIC;

   component NAND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   U8 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi7_R, tHL => twdi7_F)
	port map( Input => i7, Output => connect(7));

   -- Intrinsic delay buffers
   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_q_R, tHL => tpdi5_q_F)
	port map( Input => connect(5), Output => prop_q(5));

   U15 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_q_R, tHL => tpdi6_q_F)
	port map( Input => connect(6), Output => prop_q(6));

   U16 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi7_q_R, tHL => tpdi7_q_F)
	port map( Input => connect(7), Output => prop_q(7));

   -- Netlist
   U17 : NAND4MAC
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => q);

   U18 : NAND2MAC
	port map( I0 => prop_q(6), I1 => prop_q(7), Y => n4);

   U19 : NAND2MAC
	port map( I0 => prop_q(4), I1 => prop_q(5), Y => n3);

   U20 : NAND2MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), Y => n2);

   U21 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa2a2a2a24_x2_FTSM of oa2a2a2a24_x2 is
   for FTSM
   end for;
end CFG_oa2a2a2a24_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2a2a2a24_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi7_q_R: constant is "U16/tLH";
   attribute PROPAGATE_VALUE of tpdi7_q_F: constant is "U16/tHL";
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is "U15/tLH";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is "U15/tHL";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of twdi7_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi7_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 7) := (others => 'U');
   signal n1, n2, n3, n4 : STD_LOGIC;

   component NAND4MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         I3                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   U8 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi7_R, tHL => twdi7_F)
	port map( Input => i7, Output => connect(7));

   -- Intrinsic delay buffers
   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_q_R, tHL => tpdi5_q_F)
	port map( Input => connect(5), Output => prop_q(5));

   U15 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_q_R, tHL => tpdi6_q_F)
	port map( Input => connect(6), Output => prop_q(6));

   U16 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi7_q_R, tHL => tpdi7_q_F)
	port map( Input => connect(7), Output => prop_q(7));

   -- Netlist
   U17 : NAND4MAC
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => q);

   U18 : NAND2MAC
	port map( I0 => prop_q(6), I1 => prop_q(7), Y => n4);

   U19 : NAND2MAC
	port map( I0 => prop_q(4), I1 => prop_q(5), Y => n3);

   U20 : NAND2MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), Y => n2);

   U21 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa2a2a2a24_x4_FTSM of oa2a2a2a24_x4 is
   for FTSM
   end for;
end CFG_oa2a2a2a24_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2a2a23_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   -- Intrinsic delay buffers
   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_q_R, tHL => tpdi5_q_F)
	port map( Input => connect(5), Output => prop_q(5));

   -- Netlist
   U13 : NAND3MAC
	port map( I0 => n1, I1 => n2, I2 => n3, Y => q);

   U14 : NAND2MAC
	port map( I0 => prop_q(4), I1 => prop_q(5), Y => n2);

   U15 : NAND2MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), Y => n1);

   U16 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n3);


end FTSM;

configuration CFG_oa2a2a23_x2_FTSM of oa2a2a23_x2 is
   for FTSM
   end for;
end CFG_oa2a2a23_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2a2a23_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 5) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   -- Intrinsic delay buffers
   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_q_R, tHL => tpdi5_q_F)
	port map( Input => connect(5), Output => prop_q(5));

   -- Netlist
   U13 : NAND3MAC
	port map( I0 => n1, I1 => n2, I2 => n3, Y => q);

   U14 : NAND2MAC
	port map( I0 => prop_q(4), I1 => prop_q(5), Y => n2);

   U15 : NAND2MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), Y => n1);

   U16 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n3);


end FTSM;

configuration CFG_oa2a2a23_x4_FTSM of oa2a2a23_x4 is
   for FTSM
   end for;
end CFG_oa2a2a23_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2a22_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U10 : NAND2MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), Y => n2);

   U11 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa2a22_x2_FTSM of oa2a22_x2 is
   for FTSM
   end for;
end CFG_oa2a22_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2a22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   -- Intrinsic delay buffers
   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   -- Netlist
   U9 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U10 : NAND2MAC
	port map( I0 => prop_q(2), I1 => prop_q(3), Y => n2);

   U11 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa2a22_x4_FTSM of oa2a22_x4 is
   for FTSM
   end for;
end CFG_oa2a22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2ao222_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   -- Netlist
   U11 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U12 : OR2MAC
	port map( I0 => prop_q(3), I1 => prop_q(2), Y => n3);

   U13 : NAND2MAC
	port map( I0 => prop_q(4), I1 => n3, Y => n2);

   U14 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa2ao222_x2_FTSM of oa2ao222_x2 is
   for FTSM
   end for;
end CFG_oa2ao222_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa2ao222_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 4) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   -- Intrinsic delay buffers
   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U7 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   -- Netlist
   U11 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U12 : OR2MAC
	port map( I0 => prop_q(3), I1 => prop_q(2), Y => n3);

   U13 : NAND2MAC
	port map( I0 => prop_q(4), I1 => n3, Y => n2);

   U14 : NAND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa2ao222_x4_FTSM of oa2ao222_x4 is
   for FTSM
   end for;
end CFG_oa2ao222_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa3ao322_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   -- Intrinsic delay buffers
   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_q_R, tHL => tpdi5_q_F)
	port map( Input => connect(5), Output => prop_q(5));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_q_R, tHL => tpdi6_q_F)
	port map( Input => connect(6), Output => prop_q(6));

   -- Netlist
   U15 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U16 : OR3MAC
	port map( I0 => prop_q(3), I1 => prop_q(4), I2 => prop_q(5), Y => 
         n3);

   U17 : NAND3MAC
	port map( I0 => prop_q(1), I1 => prop_q(2), I2 => prop_q(0), Y => 
         n2);

   U18 : NAND2MAC
	port map( I0 => prop_q(6), I1 => n3, Y => n1);


end FTSM;

configuration CFG_oa3ao322_x2_FTSM of oa3ao322_x2 is
   for FTSM
   end for;
end CFG_oa3ao322_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa3ao322_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi6_q_R: constant is "U14/tLH";
   attribute PROPAGATE_VALUE of tpdi6_q_F: constant is "U14/tHL";
   attribute PROPAGATE_VALUE of tpdi5_q_R: constant is "U13/tLH";
   attribute PROPAGATE_VALUE of tpdi5_q_F: constant is "U13/tHL";
   attribute PROPAGATE_VALUE of tpdi4_q_R: constant is "U12/tLH";
   attribute PROPAGATE_VALUE of tpdi4_q_F: constant is "U12/tHL";
   attribute PROPAGATE_VALUE of tpdi3_q_R: constant is "U11/tLH";
   attribute PROPAGATE_VALUE of tpdi3_q_F: constant is "U11/tHL";
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U10/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U10/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U9/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U9/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U8/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U8/tHL";
   attribute PROPAGATE_VALUE of twdi6_F: constant is "U7/tHL";
   attribute PROPAGATE_VALUE of twdi6_R: constant is "U7/tLH";
   attribute PROPAGATE_VALUE of twdi5_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of twdi5_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of twdi4_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of twdi4_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of twdi3_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi3_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 6) := (others => 'U');
   signal n1, n2, n3 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component OR3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component NAND3MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         I2                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi3_R, tHL => twdi3_F)
	port map( Input => i3, Output => connect(3));

   U5 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi4_R, tHL => twdi4_F)
	port map( Input => i4, Output => connect(4));

   U6 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi5_R, tHL => twdi5_F)
	port map( Input => i5, Output => connect(5));

   U7 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi6_R, tHL => twdi6_F)
	port map( Input => i6, Output => connect(6));

   -- Intrinsic delay buffers
   U8 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U9 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U10 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   U11 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi3_q_R, tHL => tpdi3_q_F)
	port map( Input => connect(3), Output => prop_q(3));

   U12 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi4_q_R, tHL => tpdi4_q_F)
	port map( Input => connect(4), Output => prop_q(4));

   U13 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi5_q_R, tHL => tpdi5_q_F)
	port map( Input => connect(5), Output => prop_q(5));

   U14 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi6_q_R, tHL => tpdi6_q_F)
	port map( Input => connect(6), Output => prop_q(6));

   -- Netlist
   U15 : NAND2MAC
	port map( I0 => n1, I1 => n2, Y => q);

   U16 : OR3MAC
	port map( I0 => prop_q(3), I1 => prop_q(4), I2 => prop_q(5), Y => 
         n3);

   U17 : NAND3MAC
	port map( I0 => prop_q(1), I1 => prop_q(2), I2 => prop_q(0), Y => 
         n2);

   U18 : NAND2MAC
	port map( I0 => prop_q(6), I1 => n3, Y => n1);


end FTSM;

configuration CFG_oa3ao322_x4_FTSM of oa3ao322_x4 is
   for FTSM
   end for;
end CFG_oa3ao322_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa22_x2 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : OR2MAC
	port map( I0 => n1, I1 => prop_q(2), Y => q);

   U8 : AND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa22_x2_FTSM of oa22_x2 is
   for FTSM
   end for;
end CFG_oa22_x2_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of oa22_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi2_q_R: constant is "U6/tLH";
   attribute PROPAGATE_VALUE of tpdi2_q_F: constant is "U6/tHL";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U5/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U5/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdi2_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi2_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 2) := (others => 'U');
   signal n1 : STD_LOGIC;

   component OR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component AND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi2_R, tHL => twdi2_F)
	port map( Input => i2, Output => connect(2));

   -- Intrinsic delay buffers
   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_R, tHL => tpdi0_q_F)
	port map( Input => connect(0), Output => prop_q(0));

   U5 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   U6 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi2_q_R, tHL => tpdi2_q_F)
	port map( Input => connect(2), Output => prop_q(2));

   -- Netlist
   U7 : OR2MAC
	port map( I0 => n1, I1 => prop_q(2), Y => q);

   U8 : AND2MAC
	port map( I0 => prop_q(0), I1 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_oa22_x4_FTSM of oa22_x4 is
   for FTSM
   end for;
end CFG_oa22_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of on12_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal n1 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_F, tHL => tpdi0_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : NAND2MAC
	port map( I0 => prop_q(0), I1 => n1, Y => q);

   U6 : INVMAC
	port map( I0 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_on12_x1_FTSM of on12_x1 is
   for FTSM
   end for;
end CFG_on12_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of on12_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal n1 : STD_LOGIC;

   component NAND2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component INVMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_F, tHL => tpdi0_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_R, tHL => tpdi1_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : NAND2MAC
	port map( I0 => prop_q(0), I1 => n1, Y => q);

   U6 : INVMAC
	port map( I0 => prop_q(1), Y => n1);


end FTSM;

configuration CFG_on12_x4_FTSM of on12_x4 is
   for FTSM
   end for;
end CFG_on12_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of one_x0 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes


begin

   -- Netlist
   q <= '1';

end FTSM;

configuration CFG_one_x0_FTSM of one_x0 is
   for FTSM
   end for;
end CFG_one_x0_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of sff1_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of thck_i: constant is "FEC/F2/tHold";
   attribute PROPAGATE_VALUE of tsui_ck: constant is "FEC/F1/tSetup";
   attribute PROPAGATE_VALUE of tpdck_q_F: constant is "U3/U1/tHL";
   attribute PROPAGATE_VALUE of tpdck_q_R: constant is "U3/U1/tLH";
   attribute PROPAGATE_VALUE of twdck_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdck_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal n1 : STD_LOGIC;

   component DFFLMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         D                              :  in    STD_LOGIC;
         CLK                            :  in    STD_LOGIC;
         CLR                            :  in    STD_LOGIC;
         Q                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdck_R, tHL => twdck_F)
	port map( Input => ck, Output => connect(1));

   -- Netlist
   U3 : DFFLMAC
	generic map( tpdY_R => tpdck_q_R, tpdY_F => tpdck_q_F )
	port map( D => connect(0), CLK => connect(1), CLR => n1, Q => q);

   n1 <= '1';

   -- Forbidden Events
   FEC : if Timing_mesg generate

     F1 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tSetup => tsui_ck)
	  port map( Data(1) => connect(0), Clock => connect(1));

     F2 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tHold => thck_i)
	  port map( Data(1) => connect(0), Clock => connect(1));

   end generate FEC;

end FTSM;

configuration CFG_sff1_x4_FTSM of sff1_x4 is
   for FTSM
   end for;
end CFG_sff1_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of sff2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of thck_cmd: constant is "FEC/F6/tHold";
   attribute PROPAGATE_VALUE of tsucmd_ck: constant is "FEC/F5/tSetup";
   attribute PROPAGATE_VALUE of thck_i1: constant is "FEC/F4/tHold";
   attribute PROPAGATE_VALUE of tsui1_ck: constant is "FEC/F3/tSetup";
   attribute PROPAGATE_VALUE of thck_i0: constant is "FEC/F2/tHold";
   attribute PROPAGATE_VALUE of tsui0_ck: constant is "FEC/F1/tSetup";
   attribute PROPAGATE_VALUE of tpdck_q_F: constant is "U6/U1/tHL";
   attribute PROPAGATE_VALUE of tpdck_q_R: constant is "U6/U1/tLH";
   attribute PROPAGATE_VALUE of twdck_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of twdck_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 3) := (others => 'U');
   signal n1, n2 : STD_LOGIC;

   component MUX2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         S0                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

   component DFFLMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         D                              :  in    STD_LOGIC;
         CLK                            :  in    STD_LOGIC;
         CLR                            :  in    STD_LOGIC;
         Q                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   U3 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(2));

   U4 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdck_R, tHL => twdck_F)
	port map( Input => ck, Output => connect(3));

   -- Netlist
   U5 : MUX2MAC
	port map( I0 => connect(0), I1 => connect(1), S0 => connect(2), Y => 
         n1);

   U6 : DFFLMAC
	generic map( tpdY_R => tpdck_q_R, tpdY_F => tpdck_q_F )
	port map( D => n1, CLK => connect(3), CLR => n2, Q => q);

   n2 <= '1';

   -- Forbidden Events
   FEC : if Timing_mesg generate

     F1 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tSetup => tsui0_ck)
	  port map( Data(1) => connect(0), Clock => connect(3));

     F2 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tHold => thck_i0)
	  port map( Data(1) => connect(0), Clock => connect(3));

     F3 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tSetup => tsui1_ck)
	  port map( Data(1) => connect(1), Clock => connect(3));

     F4 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tHold => thck_i1)
	  port map( Data(1) => connect(1), Clock => connect(3));

     F5 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tSetup => tsucmd_ck)
	  port map( Data(1) => connect(2), Clock => connect(3));

     F6 : IEEE.STD_LOGIC_COMPONENTS.SUHDCK
	  generic map( N => 1, tHold => thck_cmd)
	  port map( Data(1) => connect(2), Clock => connect(3));

   end generate FEC;

end FTSM;

configuration CFG_sff2_x4_FTSM of sff2_x4 is
   for FTSM
   end for;
end CFG_sff2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of ts_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is "U3/tHL, U3/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_q_HZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_LZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component BUF3SHEMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         OE                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(1));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_q_R, tHL => tpdcmd_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_q_R, tHL => tpdi_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : BUF3SHEMAC
	port map( I0 => prop_q(1), OE => prop_q(0), Y => q);


end FTSM;

configuration CFG_ts_x4_FTSM of ts_x4 is
   for FTSM
   end for;
end CFG_ts_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of ts_x8 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi_q_R: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi_q_F: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_R: constant is "U3/tHL, U3/tLH";
   attribute PROPAGATE_VALUE of tpdcmd_q_HZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_LZ: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of tpdcmd_q_F: constant is "U3/tLH, U3/tHL";
   attribute PROPAGATE_VALUE of twdcmd_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdcmd_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component BUF3SHEMAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         OE                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi_R, tHL => twdi_F)
	port map( Input => i, Output => connect(1));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdcmd_R, tHL => twdcmd_F)
	port map( Input => cmd, Output => connect(0));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdcmd_q_R, tHL => tpdcmd_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi_q_R, tHL => tpdi_q_F)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : BUF3SHEMAC
	port map( I0 => prop_q(1), OE => prop_q(0), Y => q);


end FTSM;

configuration CFG_ts_x8_FTSM of ts_x8 is
   for FTSM
   end for;
end CFG_ts_x8_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of xr2_x1 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component XOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_F, tHL => tpdi0_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_F, tHL => tpdi1_q_R)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : XOR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => q);


end FTSM;

configuration CFG_xr2_x1_FTSM of xr2_x1 is
   for FTSM
   end for;
end CFG_xr2_x1_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of xr2_x4 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes
   attribute PROPAGATE_VALUE of tpdi1_q_F: constant is "U4/tLH";
   attribute PROPAGATE_VALUE of tpdi1_q_R: constant is "U4/tHL";
   attribute PROPAGATE_VALUE of tpdi0_q_F: constant is "U3/tLH";
   attribute PROPAGATE_VALUE of tpdi0_q_R: constant is "U3/tHL";
   attribute PROPAGATE_VALUE of twdi1_F: constant is "U2/tHL";
   attribute PROPAGATE_VALUE of twdi1_R: constant is "U2/tLH";
   attribute PROPAGATE_VALUE of twdi0_F: constant is "U1/tHL";
   attribute PROPAGATE_VALUE of twdi0_R: constant is "U1/tLH";

   signal connect : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');
   signal prop_q : STD_LOGIC_VECTOR (0 to 1) := (others => 'U');

   component XOR2MAC
      generic(
         tpdY_R                         :  Time := 0 ns;
         tpdY_F                         :  Time := 0 ns;
         strn                           :  STRENGTH := strn_X01);
      port(
         I0                             :  in    STD_LOGIC;
         I1                             :  in    STD_LOGIC;
         Y                              :  out   STD_LOGIC);
   end component;

begin

   -- Extrinsic delay buffers
   U1 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi0_R, tHL => twdi0_F)
	port map( Input => i0, Output => connect(0));

   U2 : IEEE.STD_LOGIC_COMPONENTS.WBUFGATE
	generic map( tLH => twdi1_R, tHL => twdi1_F)
	port map( Input => i1, Output => connect(1));

   -- Intrinsic delay buffers
   U3 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi0_q_F, tHL => tpdi0_q_R)
	port map( Input => connect(0), Output => prop_q(0));

   U4 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => tpdi1_q_F, tHL => tpdi1_q_R)
	port map( Input => connect(1), Output => prop_q(1));

   -- Netlist
   U5 : XOR2MAC
	port map( I0 => prop_q(1), I1 => prop_q(0), Y => q);


end FTSM;

configuration CFG_xr2_x4_FTSM of xr2_x4 is
   for FTSM
   end for;
end CFG_xr2_x4_FTSM;


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
use SYNOPSYS.attributes.backplane;
use SYNOPSYS.attributes.PRIVATE;
use SYNOPSYS.attributes.ASIC_CELL;
use SYNOPSYS.attributes.PROPAGATE_VALUE;

architecture FTSM of zero_x0 is
   attribute backplane of FTSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of FTSM : architecture is TRUE;
   attribute ASIC_CELL of FTSM : architecture is TRUE;

   -- Backannotation attributes


begin

   -- Netlist
   nq <= '0';

end FTSM;

configuration CFG_zero_x0_FTSM of zero_x0 is
   for FTSM
   end for;
end CFG_zero_x0_FTSM;


---- end of library ----
