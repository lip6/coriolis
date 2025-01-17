
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler 1999.10
-- FILENAME     :    sxlib_components.vhd
-- FILE CONTENTS:    Component Package
-- DATE CREATED :    Mon May  7 10:19:50 2001
-- 
-- LIBRARY      :    sxlib
-- DATE ENTERED :    Thu Dec 21 11:24:55 MET 2000
-- REVISION     :    1.200000
-- TECHNOLOGY   :    cmos
-- TIME SCALE   :    1 ns
-- LOGIC SYSTEM :    IEEE-1164
-- NOTES        :    
-- HISTORY      :
-- 
----------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;
-- synopsys translate_off
use IEEE.GS_TYPES.sdt_values_t;
-- synopsys translate_on

package COMPONENTS is

constant Default_Timing_mesg : Boolean := True;
constant Default_Timing_xgen : Boolean := False;

----- Component a2_x2 -----
component a2_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.261 ns;
      tpdi0_q_F                      :	Time := 0.388 ns;
      tpdi1_q_R                      :	Time := 0.203 ns;
      tpdi1_q_F                      :	Time := 0.434 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component a2_x4 -----
component a2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.338 ns;
      tpdi0_q_F                      :	Time := 0.476 ns;
      tpdi1_q_R                      :	Time := 0.269 ns;
      tpdi1_q_F                      :	Time := 0.518 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component a3_x2 -----
component a3_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component a3_x4 -----
component a3_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component a4_x2 -----
component a4_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component a4_x4 -----
component a4_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component an12_x1 -----
component an12_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.200 ns;
      tpdi0_q_F                      :	Time := 0.168 ns;
      tpdi1_q_R                      :	Time := 0.285 ns;
      tpdi1_q_F                      :	Time := 0.405 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component an12_x4 -----
component an12_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.461 ns;
      tpdi0_q_F                      :	Time := 0.471 ns;
      tpdi1_q_R                      :	Time := 0.269 ns;
      tpdi1_q_F                      :	Time := 0.518 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component ao2o22_x2 -----
component ao2o22_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component ao2o22_x4 -----
component ao2o22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component ao22_x2 -----
component ao22_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component ao22_x4 -----
component ao22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component buf_x2 -----
component buf_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_q_R                       :	Time := 0.409 ns;
      tpdi_q_F                       :	Time := 0.391 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component buf_x4 -----
component buf_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_q_R                       :	Time := 0.379 ns;
      tpdi_q_F                       :	Time := 0.409 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component buf_x8 -----
component buf_x8
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_q_R                       :	Time := 0.343 ns;
      tpdi_q_F                       :	Time := 0.396 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component inv_x1 -----
component inv_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_nq_R                      :	Time := 0.101 ns;
      tpdi_nq_F                      :	Time := 0.139 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component inv_x2 -----
component inv_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_nq_R                      :	Time := 0.069 ns;
      tpdi_nq_F                      :	Time := 0.163 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component inv_x4 -----
component inv_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_nq_R                      :	Time := 0.071 ns;
      tpdi_nq_F                      :	Time := 0.143 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component inv_x8 -----
component inv_x8
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi_nq_R                      :	Time := 0.086 ns;
      tpdi_nq_F                      :	Time := 0.133 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component mx2_x2 -----
component mx2_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component mx2_x4 -----
component mx2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component mx3_x2 -----
component mx3_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component mx3_x4 -----
component mx3_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component na2_x1 -----
component na2_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_nq_R                     :	Time := 0.059 ns;
      tpdi0_nq_F                     :	Time := 0.288 ns;
      tpdi1_nq_R                     :	Time := 0.111 ns;
      tpdi1_nq_F                     :	Time := 0.234 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component na2_x4 -----
component na2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_nq_R                     :	Time := 0.412 ns;
      tpdi0_nq_F                     :	Time := 0.552 ns;
      tpdi1_nq_R                     :	Time := 0.353 ns;
      tpdi1_nq_F                     :	Time := 0.601 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component na3_x1 -----
component na3_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component na3_x4 -----
component na3_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component na4_x1 -----
component na4_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component na4_x4 -----
component na4_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nao2o22_x1 -----
component nao2o22_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nao2o22_x4 -----
component nao2o22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nao22_x1 -----
component nao22_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nao22_x4 -----
component nao22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nmx2_x1 -----
component nmx2_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nmx2_x4 -----
component nmx2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd                            :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nmx3_x1 -----
component nmx3_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nmx3_x4 -----
component nmx3_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_LOGIC;
      cmd1                           :	in    STD_LOGIC;
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component no2_x1 -----
component no2_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_nq_R                     :	Time := 0.298 ns;
      tpdi0_nq_F                     :	Time := 0.121 ns;
      tpdi1_nq_R                     :	Time := 0.193 ns;
      tpdi1_nq_F                     :	Time := 0.161 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component no2_x4 -----
component no2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_nq_R                     :	Time := 0.618 ns;
      tpdi0_nq_F                     :	Time := 0.447 ns;
      tpdi1_nq_R                     :	Time := 0.522 ns;
      tpdi1_nq_F                     :	Time := 0.504 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component no3_x1 -----
component no3_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component no3_x4 -----
component no3_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component no4_x1 -----
component no4_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component no4_x4 -----
component no4_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa2a2a2a24_x1 -----
component noa2a2a2a24_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
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
end component;


----- Component noa2a2a2a24_x4 -----
component noa2a2a2a24_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
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
end component;


----- Component noa2a2a23_x1 -----
component noa2a2a23_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa2a2a23_x4 -----
component noa2a2a23_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa2a22_x1 -----
component noa2a22_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa2a22_x4 -----
component noa2a22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa2ao222_x1 -----
component noa2ao222_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa2ao222_x4 -----
component noa2ao222_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa3ao322_x1 -----
component noa3ao322_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa3ao322_x4 -----
component noa3ao322_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa22_x1 -----
component noa22_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component noa22_x4 -----
component noa22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nts_x1 -----
component nts_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nts_x2 -----
component nts_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nxr2_x1 -----
component nxr2_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_nq_R                     :	Time := 0.288 ns;
      tpdi0_nq_F                     :	Time := 0.293 ns;
      tpdi1_nq_R                     :	Time := 0.156 ns;
      tpdi1_nq_F                     :	Time := 0.327 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component nxr2_x4 -----
component nxr2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_nq_R                     :	Time := 0.522 ns;
      tpdi0_nq_F                     :	Time := 0.553 ns;
      tpdi1_nq_R                     :	Time := 0.553 ns;
      tpdi1_nq_F                     :	Time := 0.542 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      nq                             :	out   STD_LOGIC);
end component;


----- Component o2_x2 -----
component o2_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.406 ns;
      tpdi0_q_F                      :	Time := 0.310 ns;
      tpdi1_q_R                      :	Time := 0.335 ns;
      tpdi1_q_F                      :	Time := 0.364 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component o2_x4 -----
component o2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.491 ns;
      tpdi0_q_F                      :	Time := 0.394 ns;
      tpdi1_q_R                      :	Time := 0.427 ns;
      tpdi1_q_F                      :	Time := 0.464 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component o3_x2 -----
component o3_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component o3_x4 -----
component o3_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component o4_x2 -----
component o4_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component o4_x4 -----
component o4_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa2a2a2a24_x2 -----
component oa2a2a2a24_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
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
end component;


----- Component oa2a2a2a24_x4 -----
component oa2a2a2a24_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
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
end component;


----- Component oa2a2a23_x2 -----
component oa2a2a23_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa2a2a23_x4 -----
component oa2a2a23_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa2a22_x2 -----
component oa2a22_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa2a22_x4 -----
component oa2a22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa2ao222_x2 -----
component oa2ao222_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa2ao222_x4 -----
component oa2ao222_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa3ao322_x2 -----
component oa3ao322_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa3ao322_x4 -----
component oa3ao322_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      i3                             :	in    STD_LOGIC;
      i4                             :	in    STD_LOGIC;
      i5                             :	in    STD_LOGIC;
      i6                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa22_x2 -----
component oa22_x2
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component oa22_x4 -----
component oa22_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      i2                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component on12_x1 -----
component on12_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.111 ns;
      tpdi0_q_F                      :	Time := 0.234 ns;
      tpdi1_q_R                      :	Time := 0.314 ns;
      tpdi1_q_F                      :	Time := 0.291 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component on12_x4 -----
component on12_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.474 ns;
      tpdi0_q_F                      :	Time := 0.499 ns;
      tpdi1_q_R                      :	Time := 0.491 ns;
      tpdi1_q_F                      :	Time := 0.394 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component one_x0 -----
component one_x0
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen);

-- synopsys translate_on
   port(
      q                              :	out   STD_LOGIC := '1');
end component;


----- Component sff1_x4 -----
component sff1_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdck_q_R                      :	Time := 0.500 ns;
      tpdck_q_F                      :	Time := 0.500 ns;
      tsui_ck                        :	Time := 0.585 ns;
      thck_i                         :	Time := 0.000 ns;
      twdi_R                         :	Time := 0.000 ns;
      twdi_F                         :	Time := 0.000 ns;
      twdck_R                        :	Time := 0.000 ns;
      twdck_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      ck                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component sff2_x4 -----
component sff2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      ck                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component ts_x4 -----
component ts_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component ts_x8 -----
component ts_x8
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
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

-- synopsys translate_on
   port(
      i                              :	in    STD_LOGIC;
      cmd                            :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component xr2_x1 -----
component xr2_x1
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.292 ns;
      tpdi0_q_F                      :	Time := 0.293 ns;
      tpdi1_q_R                      :	Time := 0.377 ns;
      tpdi1_q_F                      :	Time := 0.261 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component xr2_x4 -----
component xr2_x4
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen;
      tpdi0_q_R                      :	Time := 0.521 ns;
      tpdi0_q_F                      :	Time := 0.560 ns;
      tpdi1_q_R                      :	Time := 0.541 ns;
      tpdi1_q_F                      :	Time := 0.657 ns;
      twdi0_R                        :	Time := 0.000 ns;
      twdi0_F                        :	Time := 0.000 ns;
      twdi1_R                        :	Time := 0.000 ns;
      twdi1_F                        :	Time := 0.000 ns);

-- synopsys translate_on
   port(
      i0                             :	in    STD_LOGIC;
      i1                             :	in    STD_LOGIC;
      q                              :	out   STD_LOGIC);
end component;


----- Component zero_x0 -----
component zero_x0
-- synopsys translate_off
   generic(
      Timing_mesg: Boolean := Default_Timing_mesg;
      Timing_xgen: Boolean := Default_Timing_xgen);

-- synopsys translate_on
   port(
      nq                             :	out   STD_LOGIC := '0');
end component;


end COMPONENTS;

---- end of components library ----
