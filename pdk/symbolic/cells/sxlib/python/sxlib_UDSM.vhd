
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler 1999.10
-- FILENAME     :    sxlib_UDSM.vhd
-- FILE CONTENTS:    Entity, Structural Architecture(UDSM),
--                   and Configuration
-- DATE CREATED :    Mon May  7 10:19:50 2001
-- 
-- LIBRARY      :    sxlib
-- DATE ENTERED :    Thu Dec 21 11:24:55 MET 2000
-- REVISION     :    1.200000
-- TECHNOLOGY   :    cmos
-- TIME SCALE   :    1 ns
-- LOGIC SYSTEM :    IEEE-1164
-- NOTES        :    UDSM
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

architecture UDSM of a2_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, Y => q);


end UDSM;

configuration CFG_a2_x2_UDSM of a2_x2 is
   for UDSM
   end for;
end CFG_a2_x2_UDSM;


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

architecture UDSM of a2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, Y => q);


end UDSM;

configuration CFG_a2_x4_UDSM of a2_x4 is
   for UDSM
   end for;
end CFG_a2_x4_UDSM;


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

architecture UDSM of a3_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i2, I2 => i0, Y => q);


end UDSM;

configuration CFG_a3_x2_UDSM of a3_x2 is
   for UDSM
   end for;
end CFG_a3_x2_UDSM;


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

architecture UDSM of a3_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i2, I2 => i0, Y => q);


end UDSM;

configuration CFG_a3_x4_UDSM of a3_x4 is
   for UDSM
   end for;
end CFG_a3_x4_UDSM;


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

architecture UDSM of a4_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, I3 => i3, Y => q);


end UDSM;

configuration CFG_a4_x2_UDSM of a4_x2 is
   for UDSM
   end for;
end CFG_a4_x2_UDSM;


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

architecture UDSM of a4_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, I3 => i3, Y => q);


end UDSM;

configuration CFG_a4_x4_UDSM of a4_x4 is
   for UDSM
   end for;
end CFG_a4_x4_UDSM;


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

architecture UDSM of an12_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => n1, Y => q);

   U2 : INVMAC
	port map( I0 => i0, Y => n1);


end UDSM;

configuration CFG_an12_x1_UDSM of an12_x1 is
   for UDSM
   end for;
end CFG_an12_x1_UDSM;


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

architecture UDSM of an12_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => n1, Y => q);

   U2 : INVMAC
	port map( I0 => i0, Y => n1);


end UDSM;

configuration CFG_an12_x4_UDSM of an12_x4 is
   for UDSM
   end for;
end CFG_an12_x4_UDSM;


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

architecture UDSM of ao2o22_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : OR2MAC
	port map( I0 => i3, I1 => i2, Y => n2);

   U3 : OR2MAC
	port map( I0 => i1, I1 => i0, Y => n1);


end UDSM;

configuration CFG_ao2o22_x2_UDSM of ao2o22_x2 is
   for UDSM
   end for;
end CFG_ao2o22_x2_UDSM;


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

architecture UDSM of ao2o22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : OR2MAC
	port map( I0 => i3, I1 => i2, Y => n2);

   U3 : OR2MAC
	port map( I0 => i1, I1 => i0, Y => n1);


end UDSM;

configuration CFG_ao2o22_x4_UDSM of ao2o22_x4 is
   for UDSM
   end for;
end CFG_ao2o22_x4_UDSM;


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

architecture UDSM of ao22_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => n1, Y => q);

   U2 : OR2MAC
	port map( I0 => i1, I1 => i0, Y => n1);


end UDSM;

configuration CFG_ao22_x2_UDSM of ao22_x2 is
   for UDSM
   end for;
end CFG_ao22_x2_UDSM;


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

architecture UDSM of ao22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => n1, Y => q);

   U2 : OR2MAC
	port map( I0 => i1, I1 => i0, Y => n1);


end UDSM;

configuration CFG_ao22_x4_UDSM of ao22_x4 is
   for UDSM
   end for;
end CFG_ao22_x4_UDSM;


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

architecture UDSM of buf_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

begin

   -- Concurrent assignments
   U1 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => 1 ns, tHL => 1 ns)
	port map( Input => i, Output => q);


end UDSM;

configuration CFG_buf_x2_UDSM of buf_x2 is
   for UDSM
   end for;
end CFG_buf_x2_UDSM;


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

architecture UDSM of buf_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

begin

   -- Concurrent assignments
   U1 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => 1 ns, tHL => 1 ns)
	port map( Input => i, Output => q);


end UDSM;

configuration CFG_buf_x4_UDSM of buf_x4 is
   for UDSM
   end for;
end CFG_buf_x4_UDSM;


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

architecture UDSM of buf_x8 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

begin

   -- Concurrent assignments
   U1 : IEEE.STD_LOGIC_COMPONENTS.BUFGATE
	generic map( tLH => 1 ns, tHL => 1 ns)
	port map( Input => i, Output => q);


end UDSM;

configuration CFG_buf_x8_UDSM of buf_x8 is
   for UDSM
   end for;
end CFG_buf_x8_UDSM;


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

architecture UDSM of inv_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, Y => nq);


end UDSM;

configuration CFG_inv_x1_UDSM of inv_x1 is
   for UDSM
   end for;
end CFG_inv_x1_UDSM;


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

architecture UDSM of inv_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, Y => nq);


end UDSM;

configuration CFG_inv_x2_UDSM of inv_x2 is
   for UDSM
   end for;
end CFG_inv_x2_UDSM;


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

architecture UDSM of inv_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, Y => nq);


end UDSM;

configuration CFG_inv_x4_UDSM of inv_x4 is
   for UDSM
   end for;
end CFG_inv_x4_UDSM;


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

architecture UDSM of inv_x8 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, Y => nq);


end UDSM;

configuration CFG_inv_x8_UDSM of inv_x8 is
   for UDSM
   end for;
end CFG_inv_x8_UDSM;


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

architecture UDSM of mx2_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, S0 => cmd, Y => q);


end UDSM;

configuration CFG_mx2_x2_UDSM of mx2_x2 is
   for UDSM
   end for;
end CFG_mx2_x2_UDSM;


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

architecture UDSM of mx2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, S0 => cmd, Y => q);


end UDSM;

configuration CFG_mx2_x4_UDSM of mx2_x4 is
   for UDSM
   end for;
end CFG_mx2_x4_UDSM;


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

architecture UDSM of mx3_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i2, I1 => i1, S0 => cmd1, Y => n1);

   U2 : MUX2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => n1, S0 => cmd0, Y => q);


end UDSM;

configuration CFG_mx3_x2_UDSM of mx3_x2 is
   for UDSM
   end for;
end CFG_mx3_x2_UDSM;


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

architecture UDSM of mx3_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i2, I1 => i1, S0 => cmd1, Y => n1);

   U2 : MUX2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => n1, S0 => cmd0, Y => q);


end UDSM;

configuration CFG_mx3_x4_UDSM of mx3_x4 is
   for UDSM
   end for;
end CFG_mx3_x4_UDSM;


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

architecture UDSM of na2_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, Y => nq);


end UDSM;

configuration CFG_na2_x1_UDSM of na2_x1 is
   for UDSM
   end for;
end CFG_na2_x1_UDSM;


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

architecture UDSM of na2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, Y => nq);


end UDSM;

configuration CFG_na2_x4_UDSM of na2_x4 is
   for UDSM
   end for;
end CFG_na2_x4_UDSM;


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

architecture UDSM of na3_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i2, I2 => i0, Y => nq);


end UDSM;

configuration CFG_na3_x1_UDSM of na3_x1 is
   for UDSM
   end for;
end CFG_na3_x1_UDSM;


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

architecture UDSM of na3_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i2, I2 => i0, Y => nq);


end UDSM;

configuration CFG_na3_x4_UDSM of na3_x4 is
   for UDSM
   end for;
end CFG_na3_x4_UDSM;


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

architecture UDSM of na4_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, I3 => i3, Y => nq);


end UDSM;

configuration CFG_na4_x1_UDSM of na4_x1 is
   for UDSM
   end for;
end CFG_na4_x1_UDSM;


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

architecture UDSM of na4_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, I3 => i3, Y => nq);


end UDSM;

configuration CFG_na4_x4_UDSM of na4_x4 is
   for UDSM
   end for;
end CFG_na4_x4_UDSM;


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

architecture UDSM of nao2o22_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : OR2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : OR2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_nao2o22_x1_UDSM of nao2o22_x1 is
   for UDSM
   end for;
end CFG_nao2o22_x1_UDSM;


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

architecture UDSM of nao2o22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : OR2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : OR2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_nao2o22_x4_UDSM of nao2o22_x4 is
   for UDSM
   end for;
end CFG_nao2o22_x4_UDSM;


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

architecture UDSM of nao22_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => n1, Y => nq);

   U2 : OR2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_nao22_x1_UDSM of nao22_x1 is
   for UDSM
   end for;
end CFG_nao22_x1_UDSM;


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

architecture UDSM of nao22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => n1, Y => nq);

   U2 : OR2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_nao22_x4_UDSM of nao22_x4 is
   for UDSM
   end for;
end CFG_nao22_x4_UDSM;


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

architecture UDSM of nmx2_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i0, I1 => i1, S0 => cmd, Y => n1);

   U2 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, Y => nq);


end UDSM;

configuration CFG_nmx2_x1_UDSM of nmx2_x1 is
   for UDSM
   end for;
end CFG_nmx2_x1_UDSM;


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

architecture UDSM of nmx2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i0, I1 => i1, S0 => cmd, Y => n1);

   U2 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, Y => nq);


end UDSM;

configuration CFG_nmx2_x4_UDSM of nmx2_x4 is
   for UDSM
   end for;
end CFG_nmx2_x4_UDSM;


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

architecture UDSM of nmx3_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i2, I1 => i1, S0 => cmd1, Y => n1);

   U2 : MUX2MAC
	port map( I0 => i0, I1 => n1, S0 => cmd0, Y => n2);

   U3 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n2, Y => nq);


end UDSM;

configuration CFG_nmx3_x1_UDSM of nmx3_x1 is
   for UDSM
   end for;
end CFG_nmx3_x1_UDSM;


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

architecture UDSM of nmx3_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i2, I1 => i1, S0 => cmd1, Y => n1);

   U2 : MUX2MAC
	port map( I0 => i0, I1 => n1, S0 => cmd0, Y => n2);

   U3 : INVMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n2, Y => nq);


end UDSM;

configuration CFG_nmx3_x4_UDSM of nmx3_x4 is
   for UDSM
   end for;
end CFG_nmx3_x4_UDSM;


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

architecture UDSM of no2_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, Y => nq);


end UDSM;

configuration CFG_no2_x1_UDSM of no2_x1 is
   for UDSM
   end for;
end CFG_no2_x1_UDSM;


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

architecture UDSM of no2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, Y => nq);


end UDSM;

configuration CFG_no2_x4_UDSM of no2_x4 is
   for UDSM
   end for;
end CFG_no2_x4_UDSM;


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

architecture UDSM of no3_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => i0, I2 => i1, Y => nq);


end UDSM;

configuration CFG_no3_x1_UDSM of no3_x1 is
   for UDSM
   end for;
end CFG_no3_x1_UDSM;


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

architecture UDSM of no3_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => i0, I2 => i1, Y => nq);


end UDSM;

configuration CFG_no3_x4_UDSM of no3_x4 is
   for UDSM
   end for;
end CFG_no3_x4_UDSM;


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

architecture UDSM of no4_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, I3 => i3, Y => nq);


end UDSM;

configuration CFG_no4_x1_UDSM of no4_x1 is
   for UDSM
   end for;
end CFG_no4_x1_UDSM;


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

architecture UDSM of no4_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, I3 => i3, Y => nq);


end UDSM;

configuration CFG_no4_x4_UDSM of no4_x4 is
   for UDSM
   end for;
end CFG_no4_x4_UDSM;


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

architecture UDSM of noa2a2a2a24_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => nq);

   U2 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n4);

   U3 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n3);

   U4 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n2);

   U5 : NAND2MAC
	port map( I0 => i6, I1 => i7, Y => n1);


end UDSM;

configuration CFG_noa2a2a2a24_x1_UDSM of noa2a2a2a24_x1 is
   for UDSM
   end for;
end CFG_noa2a2a2a24_x1_UDSM;


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

architecture UDSM of noa2a2a2a24_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => nq);

   U2 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n4);

   U3 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n3);

   U4 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n2);

   U5 : NAND2MAC
	port map( I0 => i6, I1 => i7, Y => n1);


end UDSM;

configuration CFG_noa2a2a2a24_x4_UDSM of noa2a2a2a24_x4 is
   for UDSM
   end for;
end CFG_noa2a2a2a24_x4_UDSM;


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

architecture UDSM of noa2a2a23_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, Y => nq);

   U2 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);

   U4 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n3);


end UDSM;

configuration CFG_noa2a2a23_x1_UDSM of noa2a2a23_x1 is
   for UDSM
   end for;
end CFG_noa2a2a23_x1_UDSM;


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

architecture UDSM of noa2a2a23_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, Y => nq);

   U2 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);

   U4 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n3);


end UDSM;

configuration CFG_noa2a2a23_x4_UDSM of noa2a2a23_x4 is
   for UDSM
   end for;
end CFG_noa2a2a23_x4_UDSM;


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

architecture UDSM of noa2a22_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_noa2a22_x1_UDSM of noa2a22_x1 is
   for UDSM
   end for;
end CFG_noa2a22_x1_UDSM;


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

architecture UDSM of noa2a22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_noa2a22_x4_UDSM of noa2a22_x4 is
   for UDSM
   end for;
end CFG_noa2a22_x4_UDSM;


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

architecture UDSM of noa2ao222_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : OR2MAC
	port map( I0 => i2, I1 => i3, Y => n3);

   U3 : NAND2MAC
	port map( I0 => i4, I1 => n3, Y => n2);

   U4 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_noa2ao222_x1_UDSM of noa2ao222_x1 is
   for UDSM
   end for;
end CFG_noa2ao222_x1_UDSM;


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

architecture UDSM of noa2ao222_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : OR2MAC
	port map( I0 => i2, I1 => i3, Y => n3);

   U3 : NAND2MAC
	port map( I0 => i4, I1 => n3, Y => n2);

   U4 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_noa2ao222_x4_UDSM of noa2ao222_x4 is
   for UDSM
   end for;
end CFG_noa2ao222_x4_UDSM;


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

architecture UDSM of noa3ao322_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : NAND3MAC
	port map( I0 => i1, I1 => i2, I2 => i0, Y => n2);

   U3 : OR3MAC
	port map( I0 => i3, I1 => i4, I2 => i5, Y => n3);

   U4 : NAND2MAC
	port map( I0 => i6, I1 => n3, Y => n1);


end UDSM;

configuration CFG_noa3ao322_x1_UDSM of noa3ao322_x1 is
   for UDSM
   end for;
end CFG_noa3ao322_x1_UDSM;


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

architecture UDSM of noa3ao322_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : AND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => nq);

   U2 : NAND3MAC
	port map( I0 => i1, I1 => i2, I2 => i0, Y => n2);

   U3 : OR3MAC
	port map( I0 => i3, I1 => i4, I2 => i5, Y => n3);

   U4 : NAND2MAC
	port map( I0 => i6, I1 => n3, Y => n1);


end UDSM;

configuration CFG_noa3ao322_x4_UDSM of noa3ao322_x4 is
   for UDSM
   end for;
end CFG_noa3ao322_x4_UDSM;


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

architecture UDSM of noa22_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => n1, Y => nq);

   U2 : AND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_noa22_x1_UDSM of noa22_x1 is
   for UDSM
   end for;
end CFG_noa22_x1_UDSM;


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

architecture UDSM of noa22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => n1, Y => nq);

   U2 : AND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_noa22_x4_UDSM of noa22_x4 is
   for UDSM
   end for;
end CFG_noa22_x4_UDSM;


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

architecture UDSM of nts_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : INV3SHEMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, OE => cmd, Y => nq);


end UDSM;

configuration CFG_nts_x1_UDSM of nts_x1 is
   for UDSM
   end for;
end CFG_nts_x1_UDSM;


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

architecture UDSM of nts_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : INV3SHEMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, OE => cmd, Y => nq);


end UDSM;

configuration CFG_nts_x2_UDSM of nts_x2 is
   for UDSM
   end for;
end CFG_nts_x2_UDSM;


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

architecture UDSM of nxr2_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NXOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i0, Y => nq);


end UDSM;

configuration CFG_nxr2_x1_UDSM of nxr2_x1 is
   for UDSM
   end for;
end CFG_nxr2_x1_UDSM;


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

architecture UDSM of nxr2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NXOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i0, Y => nq);


end UDSM;

configuration CFG_nxr2_x4_UDSM of nxr2_x4 is
   for UDSM
   end for;
end CFG_nxr2_x4_UDSM;


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

architecture UDSM of o2_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i0, Y => q);


end UDSM;

configuration CFG_o2_x2_UDSM of o2_x2 is
   for UDSM
   end for;
end CFG_o2_x2_UDSM;


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

architecture UDSM of o2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i0, Y => q);


end UDSM;

configuration CFG_o2_x4_UDSM of o2_x4 is
   for UDSM
   end for;
end CFG_o2_x4_UDSM;


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

architecture UDSM of o3_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, Y => q);


end UDSM;

configuration CFG_o3_x2_UDSM of o3_x2 is
   for UDSM
   end for;
end CFG_o3_x2_UDSM;


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

architecture UDSM of o3_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => i1, I2 => i2, Y => q);


end UDSM;

configuration CFG_o3_x4_UDSM of o3_x4 is
   for UDSM
   end for;
end CFG_o3_x4_UDSM;


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

architecture UDSM of o4_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => i3, I2 => i0, I3 => i1, Y => q);


end UDSM;

configuration CFG_o4_x2_UDSM of o4_x2 is
   for UDSM
   end for;
end CFG_o4_x2_UDSM;


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

architecture UDSM of o4_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i2, I1 => i3, I2 => i0, I3 => i1, Y => q);


end UDSM;

configuration CFG_o4_x4_UDSM of o4_x4 is
   for UDSM
   end for;
end CFG_o4_x4_UDSM;


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

architecture UDSM of oa2a2a2a24_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => q);

   U2 : NAND2MAC
	port map( I0 => i6, I1 => i7, Y => n4);

   U3 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n3);

   U4 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U5 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa2a2a2a24_x2_UDSM of oa2a2a2a24_x2 is
   for UDSM
   end for;
end CFG_oa2a2a2a24_x2_UDSM;


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

architecture UDSM of oa2a2a2a24_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND4MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, I3 => n4, Y => q);

   U2 : NAND2MAC
	port map( I0 => i6, I1 => i7, Y => n4);

   U3 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n3);

   U4 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U5 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa2a2a2a24_x4_UDSM of oa2a2a2a24_x4 is
   for UDSM
   end for;
end CFG_oa2a2a2a24_x4_UDSM;


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

architecture UDSM of oa2a2a23_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, Y => q);

   U2 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n1);

   U4 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n3);


end UDSM;

configuration CFG_oa2a2a23_x2_UDSM of oa2a2a23_x2 is
   for UDSM
   end for;
end CFG_oa2a2a23_x2_UDSM;


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

architecture UDSM of oa2a2a23_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND3MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, I2 => n3, Y => q);

   U2 : NAND2MAC
	port map( I0 => i4, I1 => i5, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n1);

   U4 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n3);


end UDSM;

configuration CFG_oa2a2a23_x4_UDSM of oa2a2a23_x4 is
   for UDSM
   end for;
end CFG_oa2a2a23_x4_UDSM;


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

architecture UDSM of oa2a22_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa2a22_x2_UDSM of oa2a22_x2 is
   for UDSM
   end for;
end CFG_oa2a22_x2_UDSM;


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

architecture UDSM of oa2a22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : NAND2MAC
	port map( I0 => i2, I1 => i3, Y => n2);

   U3 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa2a22_x4_UDSM of oa2a22_x4 is
   for UDSM
   end for;
end CFG_oa2a22_x4_UDSM;


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

architecture UDSM of oa2ao222_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : OR2MAC
	port map( I0 => i3, I1 => i2, Y => n3);

   U3 : NAND2MAC
	port map( I0 => i4, I1 => n3, Y => n2);

   U4 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa2ao222_x2_UDSM of oa2ao222_x2 is
   for UDSM
   end for;
end CFG_oa2ao222_x2_UDSM;


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

architecture UDSM of oa2ao222_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : OR2MAC
	port map( I0 => i3, I1 => i2, Y => n3);

   U3 : NAND2MAC
	port map( I0 => i4, I1 => n3, Y => n2);

   U4 : NAND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa2ao222_x4_UDSM of oa2ao222_x4 is
   for UDSM
   end for;
end CFG_oa2ao222_x4_UDSM;


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

architecture UDSM of oa3ao322_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : OR3MAC
	port map( I0 => i3, I1 => i4, I2 => i5, Y => n3);

   U3 : NAND3MAC
	port map( I0 => i1, I1 => i2, I2 => i0, Y => n2);

   U4 : NAND2MAC
	port map( I0 => i6, I1 => n3, Y => n1);


end UDSM;

configuration CFG_oa3ao322_x2_UDSM of oa3ao322_x2 is
   for UDSM
   end for;
end CFG_oa3ao322_x2_UDSM;


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

architecture UDSM of oa3ao322_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => n2, Y => q);

   U2 : OR3MAC
	port map( I0 => i3, I1 => i4, I2 => i5, Y => n3);

   U3 : NAND3MAC
	port map( I0 => i1, I1 => i2, I2 => i0, Y => n2);

   U4 : NAND2MAC
	port map( I0 => i6, I1 => n3, Y => n1);


end UDSM;

configuration CFG_oa3ao322_x4_UDSM of oa3ao322_x4 is
   for UDSM
   end for;
end CFG_oa3ao322_x4_UDSM;


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

architecture UDSM of oa22_x2 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => i2, Y => q);

   U2 : AND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa22_x2_UDSM of oa22_x2 is
   for UDSM
   end for;
end CFG_oa22_x2_UDSM;


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

architecture UDSM of oa22_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : OR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => n1, I1 => i2, Y => q);

   U2 : AND2MAC
	port map( I0 => i0, I1 => i1, Y => n1);


end UDSM;

configuration CFG_oa22_x4_UDSM of oa22_x4 is
   for UDSM
   end for;
end CFG_oa22_x4_UDSM;


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

architecture UDSM of on12_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => n1, Y => q);

   U2 : INVMAC
	port map( I0 => i1, Y => n1);


end UDSM;

configuration CFG_on12_x1_UDSM of on12_x1 is
   for UDSM
   end for;
end CFG_on12_x1_UDSM;


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

architecture UDSM of on12_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : NAND2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i0, I1 => n1, Y => q);

   U2 : INVMAC
	port map( I0 => i1, Y => n1);


end UDSM;

configuration CFG_on12_x4_UDSM of on12_x4 is
   for UDSM
   end for;
end CFG_on12_x4_UDSM;


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

architecture UDSM of one_x0 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

begin

   -- Netlist
   q <= '1';

end UDSM;

configuration CFG_one_x0_UDSM of one_x0 is
   for UDSM
   end for;
end CFG_one_x0_UDSM;


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

architecture UDSM of sff1_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : DFFLMAC
	generic map( tpdY_R => 2 ns, tpdY_F => 2 ns)
	port map( D => i, CLK => ck, CLR => n1, Q => q);

   n1 <= '1';

end UDSM;

configuration CFG_sff1_x4_UDSM of sff1_x4 is
   for UDSM
   end for;
end CFG_sff1_x4_UDSM;


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

architecture UDSM of sff2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : MUX2MAC
	port map( I0 => i0, I1 => i1, S0 => cmd, Y => n1);

   U2 : DFFLMAC
	generic map( tpdY_R => 2 ns, tpdY_F => 2 ns)
	port map( D => n1, CLK => ck, CLR => n2, Q => q);

   n2 <= '1';

end UDSM;

configuration CFG_sff2_x4_UDSM of sff2_x4 is
   for UDSM
   end for;
end CFG_sff2_x4_UDSM;


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

architecture UDSM of ts_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : BUF3SHEMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, OE => cmd, Y => q);


end UDSM;

configuration CFG_ts_x4_UDSM of ts_x4 is
   for UDSM
   end for;
end CFG_ts_x4_UDSM;


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

architecture UDSM of ts_x8 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : BUF3SHEMAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i, OE => cmd, Y => q);


end UDSM;

configuration CFG_ts_x8_UDSM of ts_x8 is
   for UDSM
   end for;
end CFG_ts_x8_UDSM;


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

architecture UDSM of xr2_x1 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : XOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i0, Y => q);


end UDSM;

configuration CFG_xr2_x1_UDSM of xr2_x1 is
   for UDSM
   end for;
end CFG_xr2_x1_UDSM;


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

architecture UDSM of xr2_x4 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

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

   -- Netlist
   U1 : XOR2MAC
	generic map( tpdY_R => 1 ns, tpdY_F => 1 ns)
	port map( I0 => i1, I1 => i0, Y => q);


end UDSM;

configuration CFG_xr2_x4_UDSM of xr2_x4 is
   for UDSM
   end for;
end CFG_xr2_x4_UDSM;


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

architecture UDSM of zero_x0 is
   attribute backplane of UDSM : architecture is SYNOPSYS.attributes.XP;
   attribute PRIVATE of UDSM : architecture is TRUE;
   attribute ASIC_CELL of UDSM : architecture is TRUE;

begin

   -- Netlist
   nq <= '0';

end UDSM;

configuration CFG_zero_x0_UDSM of zero_x0 is
   for UDSM
   end for;
end CFG_zero_x0_UDSM;


---- end of library ----
