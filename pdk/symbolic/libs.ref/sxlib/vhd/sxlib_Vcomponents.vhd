
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler 1999.10
-- FILENAME     :    sxlib_Vcomponents.vhd
-- FILE CONTENTS:    VITAL Component Package
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

library IEEE;
use IEEE.VITAL_Timing.all;
-- synopsys translate_on

package VCOMPONENTS is

constant DefaultTimingChecksOn : Boolean := True;
constant DefaultXon : Boolean := False;
constant DefaultMsgOn : Boolean := True;

----- Component a2_x2 -----
component a2_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.261 ns, 0.388 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.203 ns, 0.434 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component a2_x4 -----
component a2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.338 ns, 0.476 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.269 ns, 0.518 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component a3_x2 -----
component a3_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.395 ns, 0.435 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.353 ns, 0.479 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.290 ns, 0.521 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component a3_x4 -----
component a3_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.478 ns, 0.514 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.428 ns, 0.554 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.356 ns, 0.592 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component a4_x2 -----
component a4_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.374 ns, 0.578 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.441 ns, 0.539 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.482 ns, 0.498 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.506 ns, 0.455 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component a4_x4 -----
component a4_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.505 ns, 0.650 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.578 ns, 0.614 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.627 ns, 0.576 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.661 ns, 0.538 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component an12_x1 -----
component an12_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.200 ns, 0.168 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.285 ns, 0.405 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component an12_x4 -----
component an12_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.461 ns, 0.471 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.269 ns, 0.518 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component ao2o22_x2 -----
component ao2o22_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.572 ns, 0.451 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.508 ns, 0.542 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.432 ns, 0.627 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.488 ns, 0.526 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component ao2o22_x4 -----
component ao2o22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.696 ns, 0.569 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.637 ns, 0.666 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.554 ns, 0.744 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.606 ns, 0.639 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component ao22_x2 -----
component ao22_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.558 ns, 0.447 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.493 ns, 0.526 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.420 ns, 0.425 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component ao22_x4 -----
component ao22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.674 ns, 0.552 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.615 ns, 0.647 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.526 ns, 0.505 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component buf_x2 -----
component buf_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_q                        :	VitalDelayType01 := (0.409 ns, 0.391 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component buf_x4 -----
component buf_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_q                        :	VitalDelayType01 := (0.379 ns, 0.409 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component buf_x8 -----
component buf_x8
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_q                        :	VitalDelayType01 := (0.343 ns, 0.396 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component inv_x1 -----
component inv_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_nq                       :	VitalDelayType01 := (0.101 ns, 0.139 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component inv_x2 -----
component inv_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_nq                       :	VitalDelayType01 := (0.069 ns, 0.163 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component inv_x4 -----
component inv_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_nq                       :	VitalDelayType01 := (0.071 ns, 0.143 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component inv_x8 -----
component inv_x8
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i_nq                       :	VitalDelayType01 := (0.086 ns, 0.133 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component mx2_x2 -----
component mx2_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_q                      :	VitalDelayType01 := (0.484 ns, 0.522 ns);
      tpd_i0_q                       :	VitalDelayType01 := (0.451 ns, 0.469 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.451 ns, 0.469 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component mx2_x4 -----
component mx2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_q                      :	VitalDelayType01 := (0.615 ns, 0.647 ns);
      tpd_i0_q                       :	VitalDelayType01 := (0.564 ns, 0.576 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.564 ns, 0.576 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component mx3_x2 -----
component mx3_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd0_q                     :	VitalDelayType01 := (0.573 ns, 0.680 ns);
      tpd_cmd1_q                     :	VitalDelayType01 := (0.664 ns, 0.817 ns);
      tpd_i0_q                       :	VitalDelayType01 := (0.538 ns, 0.658 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.654 ns, 0.808 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.654 ns, 0.808 ns);
      tipd_cmd0                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd1                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component mx3_x4 -----
component mx3_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd0_q                     :	VitalDelayType01 := (0.683 ns, 0.779 ns);
      tpd_cmd1_q                     :	VitalDelayType01 := (0.792 ns, 0.967 ns);
      tpd_i0_q                       :	VitalDelayType01 := (0.640 ns, 0.774 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.770 ns, 0.948 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.770 ns, 0.948 ns);
      tipd_cmd0                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd1                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component na2_x1 -----
component na2_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.059 ns, 0.288 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.111 ns, 0.234 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component na2_x4 -----
component na2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.412 ns, 0.552 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.353 ns, 0.601 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component na3_x1 -----
component na3_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.119 ns, 0.363 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.171 ns, 0.316 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.193 ns, 0.265 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component na3_x4 -----
component na3_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.556 ns, 0.601 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.460 ns, 0.691 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.519 ns, 0.647 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component na4_x1 -----
component na4_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.179 ns, 0.438 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.237 ns, 0.395 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.269 ns, 0.350 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.282 ns, 0.302 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component na4_x4 -----
component na4_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.578 ns, 0.771 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.643 ns, 0.731 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.681 ns, 0.689 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.703 ns, 0.644 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nao2o22_x1 -----
component nao2o22_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.294 ns, 0.226 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.237 ns, 0.307 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.174 ns, 0.382 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nao2o22_x4 -----
component nao2o22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.734 ns, 0.644 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.666 ns, 0.717 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.664 ns, 0.721 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.607 ns, 0.807 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nao22_x1 -----
component nao22_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.294 ns, 0.226 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.165 ns, 0.238 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nao22_x4 -----
component nao22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.732 ns, 0.650 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.664 ns, 0.723 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.596 ns, 0.636 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nmx2_x1 -----
component nmx2_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_nq                     :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i0_nq                      :	VitalDelayType01 := (0.217 ns, 0.256 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.217 ns, 0.256 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nmx2_x4 -----
component nmx2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_nq                     :	VitalDelayType01 := (0.632 ns, 0.708 ns);
      tpd_i0_nq                      :	VitalDelayType01 := (0.610 ns, 0.653 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.610 ns, 0.653 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nmx3_x1 -----
component nmx3_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd0_nq                    :	VitalDelayType01 := (0.356 ns, 0.495 ns);
      tpd_cmd1_nq                    :	VitalDelayType01 := (0.414 ns, 0.566 ns);
      tpd_i0_nq                      :	VitalDelayType01 := (0.315 ns, 0.441 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.429 ns, 0.582 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.429 ns, 0.582 ns);
      tipd_cmd0                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd1                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nmx3_x4 -----
component nmx3_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd0_nq                    :	VitalDelayType01 := (0.790 ns, 0.936 ns);
      tpd_cmd1_nq                    :	VitalDelayType01 := (0.866 ns, 1.048 ns);
      tpd_i0_nq                      :	VitalDelayType01 := (0.748 ns, 0.900 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.869 ns, 1.053 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.869 ns, 1.053 ns);
      tipd_cmd0                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd1                      :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component no2_x1 -----
component no2_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.298 ns, 0.121 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.193 ns, 0.161 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component no2_x4 -----
component no2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.618 ns, 0.447 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.522 ns, 0.504 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component no3_x1 -----
component no3_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.318 ns, 0.246 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.215 ns, 0.243 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.407 ns, 0.192 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component no3_x4 -----
component no3_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.722 ns, 0.561 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.638 ns, 0.623 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.545 ns, 0.640 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component no4_x1 -----
component no4_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.330 ns, 0.340 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.230 ns, 0.320 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.419 ns, 0.333 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.499 ns, 0.271 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component no4_x4 -----
component no4_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.656 ns, 0.777 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.564 ns, 0.768 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.739 ns, 0.761 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.816 ns, 0.693 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2a2a2a24_x1 -----
component noa2a2a2a24_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.649 ns, 0.606 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.775 ns, 0.562 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.550 ns, 0.662 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.667 ns, 0.616 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.419 ns, 0.613 ns);
      tpd_i5_nq                      :	VitalDelayType01 := (0.329 ns, 0.662 ns);
      tpd_i6_nq                      :	VitalDelayType01 := (0.270 ns, 0.535 ns);
      tpd_i7_nq                      :	VitalDelayType01 := (0.200 ns, 0.591 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i7                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      i7                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2a2a2a24_x4 -----
component noa2a2a2a24_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.966 ns, 1.049 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (1.097 ns, 1.005 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.867 ns, 1.106 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.990 ns, 1.061 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.748 ns, 1.061 ns);
      tpd_i5_nq                      :	VitalDelayType01 := (0.649 ns, 1.109 ns);
      tpd_i6_nq                      :	VitalDelayType01 := (0.606 ns, 0.999 ns);
      tpd_i7_nq                      :	VitalDelayType01 := (0.525 ns, 1.052 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i7                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      i7                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2a2a23_x1 -----
component noa2a2a23_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.525 ns, 0.425 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.643 ns, 0.388 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.307 ns, 0.479 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.398 ns, 0.438 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.250 ns, 0.416 ns);
      tpd_i5_nq                      :	VitalDelayType01 := (0.178 ns, 0.464 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2a2a23_x4 -----
component noa2a2a23_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.834 ns, 0.814 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.955 ns, 0.778 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.620 ns, 0.873 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.716 ns, 0.833 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.574 ns, 0.819 ns);
      tpd_i5_nq                      :	VitalDelayType01 := (0.496 ns, 0.865 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2a22_x1 -----
component noa2a22_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.151 ns, 0.327 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.284 ns, 0.289 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.372 ns, 0.256 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2a22_x4 -----
component noa2a22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.562 ns, 0.745 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.646 ns, 0.714 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.701 ns, 0.703 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.805 ns, 0.677 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2ao222_x1 -----
component noa2ao222_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.348 ns, 0.422 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.440 ns, 0.378 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.186 ns, 0.473 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.256 ns, 0.459 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.240 ns, 0.309 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa2ao222_x4 -----
component noa2ao222_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.684 ns, 0.801 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.780 ns, 0.758 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.638 ns, 0.809 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.732 ns, 0.795 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.718 ns, 0.664 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa3ao322_x1 -----
component noa3ao322_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.396 ns, 0.616 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.486 ns, 0.552 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.546 ns, 0.488 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.196 ns, 0.599 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.264 ns, 0.608 ns);
      tpd_i5_nq                      :	VitalDelayType01 := (0.328 ns, 0.581 ns);
      tpd_i6_nq                      :	VitalDelayType01 := (0.246 ns, 0.311 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa3ao322_x4 -----
component noa3ao322_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.819 ns, 0.987 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.914 ns, 0.931 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.990 ns, 0.874 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.729 ns, 0.926 ns);
      tpd_i4_nq                      :	VitalDelayType01 := (0.821 ns, 0.924 ns);
      tpd_i5_nq                      :	VitalDelayType01 := (0.907 ns, 0.900 ns);
      tpd_i6_nq                      :	VitalDelayType01 := (0.738 ns, 0.718 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa22_x1 -----
component noa22_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.151 ns, 0.327 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.218 ns, 0.241 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component noa22_x4 -----
component noa22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.550 ns, 0.740 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.643 ns, 0.709 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.610 ns, 0.646 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nts_x1 -----
component nts_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_nq                     :	VitalDelayType01z := 
               (0.249 ns, 0.041 ns, 0.249 ns, 0.249 ns, 0.041 ns, 0.041 ns);
      tpd_i_nq                       :	VitalDelayType01 := (0.169 ns, 0.201 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nts_x2 -----
component nts_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_nq                     :	VitalDelayType01z := 
               (0.330 ns, 0.033 ns, 0.330 ns, 0.330 ns, 0.033 ns, 0.033 ns);
      tpd_i_nq                       :	VitalDelayType01 := (0.167 ns, 0.201 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nxr2_x1 -----
component nxr2_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.288 ns, 0.293 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.156 ns, 0.327 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component nxr2_x4 -----
component nxr2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_nq                      :	VitalDelayType01 := (0.522 ns, 0.553 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.553 ns, 0.542 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
end component;


----- Component o2_x2 -----
component o2_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.406 ns, 0.310 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.335 ns, 0.364 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component o2_x4 -----
component o2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.491 ns, 0.394 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.427 ns, 0.464 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component o3_x2 -----
component o3_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.494 ns, 0.407 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.430 ns, 0.482 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.360 ns, 0.506 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component o3_x4 -----
component o3_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.569 ns, 0.501 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.510 ns, 0.585 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.447 ns, 0.622 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component o4_x2 -----
component o4_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.508 ns, 0.601 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.446 ns, 0.631 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.567 ns, 0.531 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.378 ns, 0.626 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component o4_x4 -----
component o4_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.574 ns, 0.638 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.492 ns, 0.650 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.649 ns, 0.611 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.721 ns, 0.536 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2a2a2a24_x2 -----
component oa2a2a2a24_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.780 ns, 0.797 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.909 ns, 0.753 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.682 ns, 0.856 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.803 ns, 0.810 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.565 ns, 0.813 ns);
      tpd_i5_q                       :	VitalDelayType01 := (0.467 ns, 0.861 ns);
      tpd_i6_q                       :	VitalDelayType01 := (0.426 ns, 0.748 ns);
      tpd_i7_q                       :	VitalDelayType01 := (0.346 ns, 0.800 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i7                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      i7                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2a2a2a24_x4 -----
component oa2a2a2a24_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.823 ns, 0.879 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.955 ns, 0.835 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.726 ns, 0.940 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.851 ns, 0.895 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.619 ns, 0.902 ns);
      tpd_i5_q                       :	VitalDelayType01 := (0.515 ns, 0.949 ns);
      tpd_i6_q                       :	VitalDelayType01 := (0.487 ns, 0.845 ns);
      tpd_i7_q                       :	VitalDelayType01 := (0.399 ns, 0.895 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i7                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      i7                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2a2a23_x2 -----
component oa2a2a23_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.653 ns, 0.578 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.775 ns, 0.542 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.441 ns, 0.639 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.540 ns, 0.600 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.402 ns, 0.591 ns);
      tpd_i5_q                       :	VitalDelayType01 := (0.321 ns, 0.636 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2a2a23_x4 -----
component oa2a2a23_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.699 ns, 0.648 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.822 ns, 0.613 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.493 ns, 0.715 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.594 ns, 0.677 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.464 ns, 0.673 ns);
      tpd_i5_q                       :	VitalDelayType01 := (0.379 ns, 0.714 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2a22_x2 -----
component oa2a22_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.403 ns, 0.564 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.495 ns, 0.534 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.646 ns, 0.487 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.537 ns, 0.512 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2a22_x4 -----
component oa2a22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.519 ns, 0.696 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.624 ns, 0.669 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.763 ns, 0.596 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.644 ns, 0.619 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2ao222_x2 -----
component oa2ao222_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.495 ns, 0.581 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.598 ns, 0.539 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.464 ns, 0.604 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.556 ns, 0.578 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.558 ns, 0.453 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa2ao222_x4 -----
component oa2ao222_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.553 ns, 0.657 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.662 ns, 0.616 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.552 ns, 0.693 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.640 ns, 0.660 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.656 ns, 0.529 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa3ao322_x2 -----
component oa3ao322_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.638 ns, 0.820 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.735 ns, 0.764 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.806 ns, 0.707 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.560 ns, 0.765 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.649 ns, 0.760 ns);
      tpd_i5_q                       :	VitalDelayType01 := (0.734 ns, 0.734 ns);
      tpd_i6_q                       :	VitalDelayType01 := (0.563 ns, 0.540 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa3ao322_x4 -----
component oa3ao322_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.717 ns, 0.946 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.818 ns, 0.890 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.894 ns, 0.834 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.673 ns, 0.898 ns);
      tpd_i4_q                       :	VitalDelayType01 := (0.758 ns, 0.896 ns);
      tpd_i5_q                       :	VitalDelayType01 := (0.839 ns, 0.865 ns);
      tpd_i6_q                       :	VitalDelayType01 := (0.684 ns, 0.651 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i4                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i5                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i6                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa22_x2 -----
component oa22_x2
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.390 ns, 0.555 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.488 ns, 0.525 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.438 ns, 0.454 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component oa22_x4 -----
component oa22_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.511 ns, 0.677 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.615 ns, 0.650 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.523 ns, 0.571 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component on12_x1 -----
component on12_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.111 ns, 0.234 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.314 ns, 0.291 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component on12_x4 -----
component on12_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.474 ns, 0.499 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.491 ns, 0.394 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component one_x0 -----
component one_x0
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn);

-- synopsys translate_on
   port(
      q                              :	out   STD_ULOGIC := '1');
end component;


----- Component sff1_x4 -----
component sff1_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_ck_q                       :	VitalDelayType01 := (0.500 ns, 0.500 ns);
      tsetup_i_ck                    :	VitalDelayType := 0.585 ns;
      thold_i_ck                     :	VitalDelayType := 0.000 ns;
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_ck                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      ck                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component sff2_x4 -----
component sff2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_ck_q                       :	VitalDelayType01 := (0.500 ns, 0.500 ns);
      tsetup_i0_ck                   :	VitalDelayType := 0.764 ns;
      thold_i0_ck                    :	VitalDelayType := 0.000 ns;
      tsetup_i1_ck                   :	VitalDelayType := 0.764 ns;
      thold_i1_ck                    :	VitalDelayType := 0.000 ns;
      tsetup_cmd_ck                  :	VitalDelayType := 0.833 ns;
      thold_cmd_ck                   :	VitalDelayType := 0.000 ns;
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_ck                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      ck                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component ts_x4 -----
component ts_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_q                      :	VitalDelayType01z := 
               (0.492 ns, 0.409 ns, 0.492 ns, 0.492 ns, 0.409 ns, 0.409 ns);
      tpd_i_q                        :	VitalDelayType01 := (0.475 ns, 0.444 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component ts_x8 -----
component ts_x8
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_cmd_q                      :	VitalDelayType01z := 
               (0.626 ns, 0.466 ns, 0.626 ns, 0.626 ns, 0.466 ns, 0.466 ns);
      tpd_i_q                        :	VitalDelayType01 := (0.613 ns, 0.569 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component xr2_x1 -----
component xr2_x1
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.292 ns, 0.293 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.377 ns, 0.261 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component xr2_x4 -----
component xr2_x4
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn;
      tpd_i0_q                       :	VitalDelayType01 := (0.521 ns, 0.560 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.541 ns, 0.657 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

-- synopsys translate_on
   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
end component;


----- Component zero_x0 -----
component zero_x0
-- synopsys translate_off
   generic(
      TimingChecksOn: Boolean := DefaultTimingChecksOn;
      InstancePath: STRING := "*";
      Xon: Boolean := DefaultXon;
      MsgOn: Boolean := DefaultMsgOn);

-- synopsys translate_on
   port(
      nq                             :	out   STD_ULOGIC := '0');
end component;


end VCOMPONENTS;

---- end of VITAL components library ----
