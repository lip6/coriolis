
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler 1999.10
-- FILENAME     :    sxlib_VITAL.vhd
-- FILE CONTENTS:    Entity, Structural Architecture(VITAL),
--                   and Configuration
-- DATE CREATED :    Mon May  7 10:19:50 2001
-- 
-- LIBRARY      :    sxlib
-- DATE ENTERED :    Thu Dec 21 11:24:55 MET 2000
-- REVISION     :    1.200000
-- TECHNOLOGY   :    cmos
-- TIME SCALE   :    1 ns
-- LOGIC SYSTEM :    IEEE-1164
-- NOTES        :    VITAL, TimingChecksOn(TRUE), XGenerationOn(FALSE), TimingMessage(TRUE), OnDetect 
-- HISTORY      :
-- 
----------------------------------------------------------------

----- CELL a2_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity a2_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.261 ns, 0.388 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.203 ns, 0.434 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of a2_x2 : entity is TRUE;
end a2_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of a2_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND (i0_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_a2_x2_VITAL of a2_x2 is
   for VITAL
   end for;
end CFG_a2_x2_VITAL;


----- CELL a2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity a2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.338 ns, 0.476 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.269 ns, 0.518 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of a2_x4 : entity is TRUE;
end a2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of a2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND (i0_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_a2_x4_VITAL of a2_x4 is
   for VITAL
   end for;
end CFG_a2_x4_VITAL;


----- CELL a3_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity a3_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.395 ns, 0.435 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.353 ns, 0.479 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.290 ns, 0.521 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of a3_x2 : entity is TRUE;
end a3_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of a3_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND (i0_ipd) AND (i2_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_a3_x2_VITAL of a3_x2 is
   for VITAL
   end for;
end CFG_a3_x2_VITAL;


----- CELL a3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity a3_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.478 ns, 0.514 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.428 ns, 0.554 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.356 ns, 0.592 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of a3_x4 : entity is TRUE;
end a3_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of a3_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND (i0_ipd) AND (i2_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_a3_x4_VITAL of a3_x4 is
   for VITAL
   end for;
end CFG_a3_x4_VITAL;


----- CELL a4_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity a4_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.374 ns, 0.578 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.441 ns, 0.539 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.482 ns, 0.498 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.506 ns, 0.455 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of a4_x2 : entity is TRUE;
end a4_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of a4_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND (i0_ipd) AND (i2_ipd) AND (i3_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_a4_x2_VITAL of a4_x2 is
   for VITAL
   end for;
end CFG_a4_x2_VITAL;


----- CELL a4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity a4_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.505 ns, 0.650 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.578 ns, 0.614 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.627 ns, 0.576 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.661 ns, 0.538 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of a4_x4 : entity is TRUE;
end a4_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of a4_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND (i0_ipd) AND (i2_ipd) AND (i3_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_a4_x4_VITAL of a4_x4 is
   for VITAL
   end for;
end CFG_a4_x4_VITAL;


----- CELL an12_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity an12_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.200 ns, 0.168 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.285 ns, 0.405 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of an12_x1 : entity is TRUE;
end an12_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of an12_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_an12_x1_VITAL of an12_x1 is
   for VITAL
   end for;
end CFG_an12_x1_VITAL;


----- CELL an12_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity an12_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.461 ns, 0.471 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.269 ns, 0.518 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of an12_x4 : entity is TRUE;
end an12_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of an12_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) AND ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_an12_x4_VITAL of an12_x4 is
   for VITAL
   end for;
end CFG_an12_x4_VITAL;


----- CELL ao2o22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity ao2o22_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.572 ns, 0.451 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.508 ns, 0.542 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.432 ns, 0.627 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.488 ns, 0.526 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of ao2o22_x2 : entity is TRUE;
end ao2o22_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of ao2o22_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := ((i3_ipd) OR (i2_ipd)) AND ((i1_ipd) OR (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_ao2o22_x2_VITAL of ao2o22_x2 is
   for VITAL
   end for;
end CFG_ao2o22_x2_VITAL;


----- CELL ao2o22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity ao2o22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.696 ns, 0.569 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.637 ns, 0.666 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.554 ns, 0.744 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.606 ns, 0.639 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of ao2o22_x4 : entity is TRUE;
end ao2o22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of ao2o22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := ((i3_ipd) OR (i2_ipd)) AND ((i1_ipd) OR (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_ao2o22_x4_VITAL of ao2o22_x4 is
   for VITAL
   end for;
end CFG_ao2o22_x4_VITAL;


----- CELL ao22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity ao22_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.558 ns, 0.447 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.493 ns, 0.526 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.420 ns, 0.425 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of ao22_x2 : entity is TRUE;
end ao22_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of ao22_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i2_ipd) AND ((i1_ipd) OR (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_ao22_x2_VITAL of ao22_x2 is
   for VITAL
   end for;
end CFG_ao22_x2_VITAL;


----- CELL ao22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity ao22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.674 ns, 0.552 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.615 ns, 0.647 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.526 ns, 0.505 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of ao22_x4 : entity is TRUE;
end ao22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of ao22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i2_ipd) AND ((i1_ipd) OR (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_ao22_x4_VITAL of ao22_x4 is
   for VITAL
   end for;
end CFG_ao22_x4_VITAL;


----- CELL buf_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity buf_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_q                        :	VitalDelayType01 := (0.409 ns, 0.391 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of buf_x2 : entity is TRUE;
end buf_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of buf_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := TO_X01(i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_buf_x2_VITAL of buf_x2 is
   for VITAL
   end for;
end CFG_buf_x2_VITAL;


----- CELL buf_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity buf_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_q                        :	VitalDelayType01 := (0.379 ns, 0.409 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of buf_x4 : entity is TRUE;
end buf_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of buf_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := TO_X01(i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_buf_x4_VITAL of buf_x4 is
   for VITAL
   end for;
end CFG_buf_x4_VITAL;


----- CELL buf_x8 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity buf_x8 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_q                        :	VitalDelayType01 := (0.343 ns, 0.396 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of buf_x8 : entity is TRUE;
end buf_x8;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of buf_x8 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := TO_X01(i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_buf_x8_VITAL of buf_x8 is
   for VITAL
   end for;
end CFG_buf_x8_VITAL;


----- CELL inv_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity inv_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_nq                       :	VitalDelayType01 := (0.101 ns, 0.139 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of inv_x1 : entity is TRUE;
end inv_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of inv_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := (NOT i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_inv_x1_VITAL of inv_x1 is
   for VITAL
   end for;
end CFG_inv_x1_VITAL;


----- CELL inv_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity inv_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_nq                       :	VitalDelayType01 := (0.069 ns, 0.163 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of inv_x2 : entity is TRUE;
end inv_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of inv_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := (NOT i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_inv_x2_VITAL of inv_x2 is
   for VITAL
   end for;
end CFG_inv_x2_VITAL;


----- CELL inv_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity inv_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_nq                       :	VitalDelayType01 := (0.071 ns, 0.143 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of inv_x4 : entity is TRUE;
end inv_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of inv_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := (NOT i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_inv_x4_VITAL of inv_x4 is
   for VITAL
   end for;
end CFG_inv_x4_VITAL;


----- CELL inv_x8 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity inv_x8 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i_nq                       :	VitalDelayType01 := (0.086 ns, 0.133 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of inv_x8 : entity is TRUE;
end inv_x8;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of inv_x8 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := (NOT i_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i_ipd'last_event, tpd_i_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_inv_x8_VITAL of inv_x8 is
   for VITAL
   end for;
end CFG_inv_x8_VITAL;


----- CELL mx2_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity mx2_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_q                      :	VitalDelayType01 := (0.484 ns, 0.522 ns);
      tpd_i0_q                       :	VitalDelayType01 := (0.451 ns, 0.469 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.451 ns, 0.469 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of mx2_x2 : entity is TRUE;
end mx2_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of mx2_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd_ipd, i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := VitalMUX
                 (data => (i1_ipd, i0_ipd),
                  dselect => (0 => cmd_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (cmd_ipd'last_event, tpd_cmd_q, TRUE),
                 1 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 2 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_mx2_x2_VITAL of mx2_x2 is
   for VITAL
   end for;
end CFG_mx2_x2_VITAL;


----- CELL mx2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity mx2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_q                      :	VitalDelayType01 := (0.615 ns, 0.647 ns);
      tpd_i0_q                       :	VitalDelayType01 := (0.564 ns, 0.576 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.564 ns, 0.576 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of mx2_x4 : entity is TRUE;
end mx2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of mx2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd_ipd, i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := VitalMUX
                 (data => (i1_ipd, i0_ipd),
                  dselect => (0 => cmd_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (cmd_ipd'last_event, tpd_cmd_q, TRUE),
                 1 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 2 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_mx2_x4_VITAL of mx2_x4 is
   for VITAL
   end for;
end CFG_mx2_x4_VITAL;


----- CELL mx3_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity mx3_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of mx3_x2 : entity is TRUE;
end mx3_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of mx3_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd0_ipd, cmd0, tipd_cmd0);
   VitalWireDelay (cmd1_ipd, cmd1, tipd_cmd1);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd0_ipd, cmd1_ipd, i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := VitalMUX
                 (data => (i1_ipd, i2_ipd, i0_ipd, i0_ipd),
                  dselect => (cmd0_ipd, cmd1_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (cmd0_ipd'last_event, tpd_cmd0_q, TRUE),
                 1 => (cmd1_ipd'last_event, tpd_cmd1_q, TRUE),
                 2 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 3 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 4 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_mx3_x2_VITAL of mx3_x2 is
   for VITAL
   end for;
end CFG_mx3_x2_VITAL;


----- CELL mx3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity mx3_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of mx3_x4 : entity is TRUE;
end mx3_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of mx3_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd0_ipd, cmd0, tipd_cmd0);
   VitalWireDelay (cmd1_ipd, cmd1, tipd_cmd1);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd0_ipd, cmd1_ipd, i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := VitalMUX
                 (data => (i1_ipd, i2_ipd, i0_ipd, i0_ipd),
                  dselect => (cmd0_ipd, cmd1_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (cmd0_ipd'last_event, tpd_cmd0_q, TRUE),
                 1 => (cmd1_ipd'last_event, tpd_cmd1_q, TRUE),
                 2 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 3 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 4 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_mx3_x4_VITAL of mx3_x4 is
   for VITAL
   end for;
end CFG_mx3_x4_VITAL;


----- CELL na2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity na2_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.059 ns, 0.288 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.111 ns, 0.234 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of na2_x1 : entity is TRUE;
end na2_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of na2_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) OR ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_na2_x1_VITAL of na2_x1 is
   for VITAL
   end for;
end CFG_na2_x1_VITAL;


----- CELL na2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity na2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.412 ns, 0.552 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.353 ns, 0.601 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of na2_x4 : entity is TRUE;
end na2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of na2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) OR ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_na2_x4_VITAL of na2_x4 is
   for VITAL
   end for;
end CFG_na2_x4_VITAL;


----- CELL na3_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity na3_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.119 ns, 0.363 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.171 ns, 0.316 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.193 ns, 0.265 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of na3_x1 : entity is TRUE;
end na3_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of na3_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) OR ((NOT i0_ipd)) OR ((NOT i2_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_na3_x1_VITAL of na3_x1 is
   for VITAL
   end for;
end CFG_na3_x1_VITAL;


----- CELL na3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity na3_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.556 ns, 0.601 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.460 ns, 0.691 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.519 ns, 0.647 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of na3_x4 : entity is TRUE;
end na3_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of na3_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) OR ((NOT i0_ipd)) OR ((NOT i2_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_na3_x4_VITAL of na3_x4 is
   for VITAL
   end for;
end CFG_na3_x4_VITAL;


----- CELL na4_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity na4_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.179 ns, 0.438 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.237 ns, 0.395 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.269 ns, 0.350 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.282 ns, 0.302 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of na4_x1 : entity is TRUE;
end na4_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of na4_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       ((NOT i1_ipd)) OR ((NOT i0_ipd)) OR ((NOT i2_ipd)) OR ((NOT i3_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_na4_x1_VITAL of na4_x1 is
   for VITAL
   end for;
end CFG_na4_x1_VITAL;


----- CELL na4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity na4_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.578 ns, 0.771 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.643 ns, 0.731 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.681 ns, 0.689 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.703 ns, 0.644 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of na4_x4 : entity is TRUE;
end na4_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of na4_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       ((NOT i1_ipd)) OR ((NOT i0_ipd)) OR ((NOT i2_ipd)) OR ((NOT i3_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_na4_x4_VITAL of na4_x4 is
   for VITAL
   end for;
end CFG_na4_x4_VITAL;


----- CELL nao2o22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nao2o22_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.294 ns, 0.226 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.237 ns, 0.307 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.174 ns, 0.382 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nao2o22_x1 : entity is TRUE;
end nao2o22_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nao2o22_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) AND ((NOT i2_ipd))) OR (((NOT i1_ipd)) AND ((NOT
         i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nao2o22_x1_VITAL of nao2o22_x1 is
   for VITAL
   end for;
end CFG_nao2o22_x1_VITAL;


----- CELL nao2o22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nao2o22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.734 ns, 0.644 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.666 ns, 0.717 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.664 ns, 0.721 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.607 ns, 0.807 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nao2o22_x4 : entity is TRUE;
end nao2o22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nao2o22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) AND ((NOT i2_ipd))) OR (((NOT i1_ipd)) AND ((NOT
         i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nao2o22_x4_VITAL of nao2o22_x4 is
   for VITAL
   end for;
end CFG_nao2o22_x4_VITAL;


----- CELL nao22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nao22_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.294 ns, 0.226 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.165 ns, 0.238 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nao22_x1 : entity is TRUE;
end nao22_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nao22_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i2_ipd)) OR (((NOT i1_ipd)) AND ((NOT i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nao22_x1_VITAL of nao22_x1 is
   for VITAL
   end for;
end CFG_nao22_x1_VITAL;


----- CELL nao22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nao22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.732 ns, 0.650 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.664 ns, 0.723 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.596 ns, 0.636 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nao22_x4 : entity is TRUE;
end nao22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nao22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i2_ipd)) OR (((NOT i1_ipd)) AND ((NOT i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nao22_x4_VITAL of nao22_x4 is
   for VITAL
   end for;
end CFG_nao22_x4_VITAL;


----- CELL nmx2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nmx2_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_nq                     :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i0_nq                      :	VitalDelayType01 := (0.217 ns, 0.256 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.217 ns, 0.256 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nmx2_x1 : entity is TRUE;
end nmx2_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nmx2_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd_ipd, i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := VitalMUX
                 (data => (i1_ipd, i0_ipd),
                  dselect => (0 => cmd_ipd));
      nq_zd := NOT nq_zd;

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (cmd_ipd'last_event, tpd_cmd_nq, TRUE),
                 1 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 2 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nmx2_x1_VITAL of nmx2_x1 is
   for VITAL
   end for;
end CFG_nmx2_x1_VITAL;


----- CELL nmx2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nmx2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_nq                     :	VitalDelayType01 := (0.632 ns, 0.708 ns);
      tpd_i0_nq                      :	VitalDelayType01 := (0.610 ns, 0.653 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.610 ns, 0.653 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      cmd                            :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nmx2_x4 : entity is TRUE;
end nmx2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nmx2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd_ipd, i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := VitalMUX
                 (data => (i1_ipd, i0_ipd),
                  dselect => (0 => cmd_ipd));
      nq_zd := NOT nq_zd;

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (cmd_ipd'last_event, tpd_cmd_nq, TRUE),
                 1 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 2 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nmx2_x4_VITAL of nmx2_x4 is
   for VITAL
   end for;
end CFG_nmx2_x4_VITAL;


----- CELL nmx3_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nmx3_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nmx3_x1 : entity is TRUE;
end nmx3_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nmx3_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd0_ipd, cmd0, tipd_cmd0);
   VitalWireDelay (cmd1_ipd, cmd1, tipd_cmd1);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd0_ipd, cmd1_ipd, i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := VitalMUX
                 (data => (i1_ipd, i2_ipd, i0_ipd, i0_ipd),
                  dselect => (cmd0_ipd, cmd1_ipd));
      nq_zd := NOT nq_zd;

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (cmd0_ipd'last_event, tpd_cmd0_nq, TRUE),
                 1 => (cmd1_ipd'last_event, tpd_cmd1_nq, TRUE),
                 2 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 3 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 4 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nmx3_x1_VITAL of nmx3_x1 is
   for VITAL
   end for;
end CFG_nmx3_x1_VITAL;


----- CELL nmx3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nmx3_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      cmd0                           :	in    STD_ULOGIC;
      cmd1                           :	in    STD_ULOGIC;
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nmx3_x4 : entity is TRUE;
end nmx3_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nmx3_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL cmd0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (cmd0_ipd, cmd0, tipd_cmd0);
   VitalWireDelay (cmd1_ipd, cmd1, tipd_cmd1);
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (cmd0_ipd, cmd1_ipd, i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := VitalMUX
                 (data => (i1_ipd, i2_ipd, i0_ipd, i0_ipd),
                  dselect => (cmd0_ipd, cmd1_ipd));
      nq_zd := NOT nq_zd;

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (cmd0_ipd'last_event, tpd_cmd0_nq, TRUE),
                 1 => (cmd1_ipd'last_event, tpd_cmd1_nq, TRUE),
                 2 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 3 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 4 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nmx3_x4_VITAL of nmx3_x4 is
   for VITAL
   end for;
end CFG_nmx3_x4_VITAL;


----- CELL no2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity no2_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.298 ns, 0.121 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.193 ns, 0.161 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of no2_x1 : entity is TRUE;
end no2_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of no2_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) AND ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_no2_x1_VITAL of no2_x1 is
   for VITAL
   end for;
end CFG_no2_x1_VITAL;


----- CELL no2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity no2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.618 ns, 0.447 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.522 ns, 0.504 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of no2_x4 : entity is TRUE;
end no2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of no2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) AND ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_no2_x4_VITAL of no2_x4 is
   for VITAL
   end for;
end CFG_no2_x4_VITAL;


----- CELL no3_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity no3_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.318 ns, 0.246 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.215 ns, 0.243 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.407 ns, 0.192 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of no3_x1 : entity is TRUE;
end no3_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of no3_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) AND ((NOT i0_ipd)) AND ((NOT i2_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_no3_x1_VITAL of no3_x1 is
   for VITAL
   end for;
end CFG_no3_x1_VITAL;


----- CELL no3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity no3_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.722 ns, 0.561 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.638 ns, 0.623 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.545 ns, 0.640 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of no3_x4 : entity is TRUE;
end no3_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of no3_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i1_ipd)) AND ((NOT i0_ipd)) AND ((NOT i2_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_no3_x4_VITAL of no3_x4 is
   for VITAL
   end for;
end CFG_no3_x4_VITAL;


----- CELL no4_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity no4_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.330 ns, 0.340 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.230 ns, 0.320 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.419 ns, 0.333 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.499 ns, 0.271 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of no4_x1 : entity is TRUE;
end no4_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of no4_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       ((NOT i1_ipd)) AND ((NOT i0_ipd)) AND ((NOT i2_ipd)) AND ((NOT
         i3_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_no4_x1_VITAL of no4_x1 is
   for VITAL
   end for;
end CFG_no4_x1_VITAL;


----- CELL no4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity no4_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.656 ns, 0.777 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.564 ns, 0.768 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.739 ns, 0.761 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.816 ns, 0.693 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of no4_x4 : entity is TRUE;
end no4_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of no4_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       ((NOT i1_ipd)) AND ((NOT i0_ipd)) AND ((NOT i2_ipd)) AND ((NOT
         i3_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_no4_x4_VITAL of no4_x4 is
   for VITAL
   end for;
end CFG_no4_x4_VITAL;


----- CELL noa2a2a2a24_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2a2a2a24_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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
attribute VITAL_LEVEL0 of noa2a2a2a24_x1 : entity is TRUE;
end noa2a2a2a24_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2a2a2a24_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i7_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   VitalWireDelay (i7_ipd, i7, tipd_i7);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd, i7_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) OR ((NOT i2_ipd))) AND (((NOT i1_ipd)) OR ((NOT
         i0_ipd))) AND (((NOT i5_ipd)) OR ((NOT i4_ipd))) AND (((NOT i7_ipd))
         OR ((NOT i6_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_nq, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_nq, TRUE),
                 7 => (i7_ipd'last_event, tpd_i7_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2a2a2a24_x1_VITAL of noa2a2a2a24_x1 is
   for VITAL
   end for;
end CFG_noa2a2a2a24_x1_VITAL;


----- CELL noa2a2a2a24_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2a2a2a24_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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
attribute VITAL_LEVEL0 of noa2a2a2a24_x4 : entity is TRUE;
end noa2a2a2a24_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2a2a2a24_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i7_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   VitalWireDelay (i7_ipd, i7, tipd_i7);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd, i7_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) OR ((NOT i2_ipd))) AND (((NOT i1_ipd)) OR ((NOT
         i0_ipd))) AND (((NOT i5_ipd)) OR ((NOT i4_ipd))) AND (((NOT i7_ipd))
         OR ((NOT i6_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_nq, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_nq, TRUE),
                 7 => (i7_ipd'last_event, tpd_i7_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2a2a2a24_x4_VITAL of noa2a2a2a24_x4 is
   for VITAL
   end for;
end CFG_noa2a2a2a24_x4_VITAL;


----- CELL noa2a2a23_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2a2a23_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa2a2a23_x1 : entity is TRUE;
end noa2a2a23_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2a2a23_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) OR ((NOT i2_ipd))) AND (((NOT i1_ipd)) OR ((NOT
         i0_ipd))) AND (((NOT i5_ipd)) OR ((NOT i4_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2a2a23_x1_VITAL of noa2a2a23_x1 is
   for VITAL
   end for;
end CFG_noa2a2a23_x1_VITAL;


----- CELL noa2a2a23_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2a2a23_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa2a2a23_x4 : entity is TRUE;
end noa2a2a23_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2a2a23_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) OR ((NOT i2_ipd))) AND (((NOT i1_ipd)) OR ((NOT
         i0_ipd))) AND (((NOT i5_ipd)) OR ((NOT i4_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2a2a23_x4_VITAL of noa2a2a23_x4 is
   for VITAL
   end for;
end CFG_noa2a2a23_x4_VITAL;


----- CELL noa2a22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2a22_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.151 ns, 0.327 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.284 ns, 0.289 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.372 ns, 0.256 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa2a22_x1 : entity is TRUE;
end noa2a22_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2a22_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) OR ((NOT i2_ipd))) AND (((NOT i1_ipd)) OR ((NOT
         i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2a22_x1_VITAL of noa2a22_x1 is
   for VITAL
   end for;
end CFG_noa2a22_x1_VITAL;


----- CELL noa2a22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2a22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.562 ns, 0.745 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.646 ns, 0.714 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.701 ns, 0.703 ns);
      tpd_i3_nq                      :	VitalDelayType01 := (0.805 ns, 0.677 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa2a22_x4 : entity is TRUE;
end noa2a22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2a22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i3_ipd)) OR ((NOT i2_ipd))) AND (((NOT i1_ipd)) OR ((NOT
         i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2a22_x4_VITAL of noa2a22_x4 is
   for VITAL
   end for;
end CFG_noa2a22_x4_VITAL;


----- CELL noa2ao222_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2ao222_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa2ao222_x1 : entity is TRUE;
end noa2ao222_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2ao222_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i4_ipd)) OR (((NOT i3_ipd)) AND ((NOT i2_ipd)))) AND (((NOT
         i1_ipd)) OR ((NOT i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2ao222_x1_VITAL of noa2ao222_x1 is
   for VITAL
   end for;
end CFG_noa2ao222_x1_VITAL;


----- CELL noa2ao222_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa2ao222_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa2ao222_x4 : entity is TRUE;
end noa2ao222_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa2ao222_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i4_ipd)) OR (((NOT i3_ipd)) AND ((NOT i2_ipd)))) AND (((NOT
         i1_ipd)) OR ((NOT i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa2ao222_x4_VITAL of noa2ao222_x4 is
   for VITAL
   end for;
end CFG_noa2ao222_x4_VITAL;


----- CELL noa3ao322_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa3ao322_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa3ao322_x1 : entity is TRUE;
end noa3ao322_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa3ao322_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i6_ipd)) OR (((NOT i4_ipd)) AND ((NOT i3_ipd)) AND ((NOT
         i5_ipd)))) AND (((NOT i1_ipd)) OR ((NOT i0_ipd)) OR ((NOT i2_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_nq, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa3ao322_x1_VITAL of noa3ao322_x1 is
   for VITAL
   end for;
end CFG_noa3ao322_x1_VITAL;


----- CELL noa3ao322_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa3ao322_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa3ao322_x4 : entity is TRUE;
end noa3ao322_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa3ao322_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd :=
       (((NOT i6_ipd)) OR (((NOT i4_ipd)) AND ((NOT i3_ipd)) AND ((NOT
         i5_ipd)))) AND (((NOT i1_ipd)) OR ((NOT i0_ipd)) OR ((NOT i2_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_nq, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_nq, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_nq, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa3ao322_x4_VITAL of noa3ao322_x4 is
   for VITAL
   end for;
end CFG_noa3ao322_x4_VITAL;


----- CELL noa22_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa22_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.151 ns, 0.327 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.218 ns, 0.287 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.218 ns, 0.241 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa22_x1 : entity is TRUE;
end noa22_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa22_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i2_ipd)) AND (((NOT i1_ipd)) OR ((NOT i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa22_x1_VITAL of noa22_x1 is
   for VITAL
   end for;
end CFG_noa22_x1_VITAL;


----- CELL noa22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity noa22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.550 ns, 0.740 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.643 ns, 0.709 ns);
      tpd_i2_nq                      :	VitalDelayType01 := (0.610 ns, 0.646 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of noa22_x4 : entity is TRUE;
end noa22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of noa22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := ((NOT i2_ipd)) AND (((NOT i1_ipd)) OR ((NOT i0_ipd)));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_noa22_x4_VITAL of noa22_x4 is
   for VITAL
   end for;
end CFG_noa22_x4_VITAL;


----- CELL nts_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nts_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_nq                     :	VitalDelayType01z := 
               (0.249 ns, 0.041 ns, 0.249 ns, 0.249 ns, 0.041 ns, 0.041 ns);
      tpd_i_nq                       :	VitalDelayType01 := (0.169 ns, 0.201 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nts_x1 : entity is TRUE;
end nts_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nts_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd, cmd_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := VitalBUFIF0 (data => (NOT i_ipd),
              enable => (NOT cmd_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01Z (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (cmd_ipd'last_event, VitalExtendToFillDelay(tpd_cmd_nq), TRUE),
                 1 => (i_ipd'last_event, VitalExtendToFillDelay(tpd_i_nq), TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING,
       OutputMap => "UX01ZWLH-");

end process;

end VITAL;

configuration CFG_nts_x1_VITAL of nts_x1 is
   for VITAL
   end for;
end CFG_nts_x1_VITAL;


----- CELL nts_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nts_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_nq                     :	VitalDelayType01z := 
               (0.330 ns, 0.033 ns, 0.330 ns, 0.330 ns, 0.033 ns, 0.033 ns);
      tpd_i_nq                       :	VitalDelayType01 := (0.167 ns, 0.201 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nts_x2 : entity is TRUE;
end nts_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nts_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd, cmd_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := VitalBUFIF0 (data => (NOT i_ipd),
              enable => (NOT cmd_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01Z (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (cmd_ipd'last_event, VitalExtendToFillDelay(tpd_cmd_nq), TRUE),
                 1 => (i_ipd'last_event, VitalExtendToFillDelay(tpd_i_nq), TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING,
       OutputMap => "UX01ZWLH-");

end process;

end VITAL;

configuration CFG_nts_x2_VITAL of nts_x2 is
   for VITAL
   end for;
end CFG_nts_x2_VITAL;


----- CELL nxr2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nxr2_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.288 ns, 0.293 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.156 ns, 0.327 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nxr2_x1 : entity is TRUE;
end nxr2_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nxr2_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := (i1_ipd) XOR ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nxr2_x1_VITAL of nxr2_x1 is
   for VITAL
   end for;
end CFG_nxr2_x1_VITAL;


----- CELL nxr2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity nxr2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_nq                      :	VitalDelayType01 := (0.522 ns, 0.553 ns);
      tpd_i1_nq                      :	VitalDelayType01 := (0.553 ns, 0.542 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      nq                             :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of nxr2_x4 : entity is TRUE;
end nxr2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of nxr2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS nq_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE nq_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      nq_zd := (i1_ipd) XOR ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => nq,
       GlitchData => nq_GlitchData,
       OutSignalName => "nq",
       OutTemp => nq_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_nq, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_nq, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_nxr2_x4_VITAL of nxr2_x4 is
   for VITAL
   end for;
end CFG_nxr2_x4_VITAL;


----- CELL o2_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity o2_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.406 ns, 0.310 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.335 ns, 0.364 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of o2_x2 : entity is TRUE;
end o2_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of o2_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR (i0_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_o2_x2_VITAL of o2_x2 is
   for VITAL
   end for;
end CFG_o2_x2_VITAL;


----- CELL o2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity o2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.491 ns, 0.394 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.427 ns, 0.464 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of o2_x4 : entity is TRUE;
end o2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of o2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR (i0_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_o2_x4_VITAL of o2_x4 is
   for VITAL
   end for;
end CFG_o2_x4_VITAL;


----- CELL o3_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity o3_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.494 ns, 0.407 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.430 ns, 0.482 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.360 ns, 0.506 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of o3_x2 : entity is TRUE;
end o3_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of o3_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR (i0_ipd) OR (i2_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_o3_x2_VITAL of o3_x2 is
   for VITAL
   end for;
end CFG_o3_x2_VITAL;


----- CELL o3_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity o3_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.569 ns, 0.501 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.510 ns, 0.585 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.447 ns, 0.622 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of o3_x4 : entity is TRUE;
end o3_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of o3_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR (i0_ipd) OR (i2_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_o3_x4_VITAL of o3_x4 is
   for VITAL
   end for;
end CFG_o3_x4_VITAL;


----- CELL o4_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity o4_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.508 ns, 0.601 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.446 ns, 0.631 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.567 ns, 0.531 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.378 ns, 0.626 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of o4_x2 : entity is TRUE;
end o4_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of o4_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR (i0_ipd) OR (i2_ipd) OR (i3_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_o4_x2_VITAL of o4_x2 is
   for VITAL
   end for;
end CFG_o4_x2_VITAL;


----- CELL o4_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity o4_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.574 ns, 0.638 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.492 ns, 0.650 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.649 ns, 0.611 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.721 ns, 0.536 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of o4_x4 : entity is TRUE;
end o4_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of o4_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR (i0_ipd) OR (i2_ipd) OR (i3_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_o4_x4_VITAL of o4_x4 is
   for VITAL
   end for;
end CFG_o4_x4_VITAL;


----- CELL oa2a2a2a24_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2a2a2a24_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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
attribute VITAL_LEVEL0 of oa2a2a2a24_x2 : entity is TRUE;
end oa2a2a2a24_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2a2a2a24_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i7_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   VitalWireDelay (i7_ipd, i7, tipd_i7);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd, i7_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       ((i3_ipd) AND (i2_ipd)) OR ((i1_ipd) AND (i0_ipd)) OR ((i5_ipd) AND
         (i4_ipd)) OR ((i7_ipd) AND (i6_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_q, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_q, TRUE),
                 7 => (i7_ipd'last_event, tpd_i7_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2a2a2a24_x2_VITAL of oa2a2a2a24_x2 is
   for VITAL
   end for;
end CFG_oa2a2a2a24_x2_VITAL;


----- CELL oa2a2a2a24_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2a2a2a24_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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
attribute VITAL_LEVEL0 of oa2a2a2a24_x4 : entity is TRUE;
end oa2a2a2a24_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2a2a2a24_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i7_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   VitalWireDelay (i7_ipd, i7, tipd_i7);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd, i7_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       ((i3_ipd) AND (i2_ipd)) OR ((i1_ipd) AND (i0_ipd)) OR ((i5_ipd) AND
         (i4_ipd)) OR ((i7_ipd) AND (i6_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_q, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_q, TRUE),
                 7 => (i7_ipd'last_event, tpd_i7_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2a2a2a24_x4_VITAL of oa2a2a2a24_x4 is
   for VITAL
   end for;
end CFG_oa2a2a2a24_x4_VITAL;


----- CELL oa2a2a23_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2a2a23_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa2a2a23_x2 : entity is TRUE;
end oa2a2a23_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2a2a23_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       ((i3_ipd) AND (i2_ipd)) OR ((i1_ipd) AND (i0_ipd)) OR ((i5_ipd) AND
         (i4_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2a2a23_x2_VITAL of oa2a2a23_x2 is
   for VITAL
   end for;
end CFG_oa2a2a23_x2_VITAL;


----- CELL oa2a2a23_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2a2a23_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa2a2a23_x4 : entity is TRUE;
end oa2a2a23_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2a2a23_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       ((i3_ipd) AND (i2_ipd)) OR ((i1_ipd) AND (i0_ipd)) OR ((i5_ipd) AND
         (i4_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2a2a23_x4_VITAL of oa2a2a23_x4 is
   for VITAL
   end for;
end CFG_oa2a2a23_x4_VITAL;


----- CELL oa2a22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2a22_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.403 ns, 0.564 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.495 ns, 0.534 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.646 ns, 0.487 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.537 ns, 0.512 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa2a22_x2 : entity is TRUE;
end oa2a22_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2a22_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := ((i3_ipd) AND (i2_ipd)) OR ((i1_ipd) AND (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2a22_x2_VITAL of oa2a22_x2 is
   for VITAL
   end for;
end CFG_oa2a22_x2_VITAL;


----- CELL oa2a22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2a22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.519 ns, 0.696 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.624 ns, 0.669 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.763 ns, 0.596 ns);
      tpd_i3_q                       :	VitalDelayType01 := (0.644 ns, 0.619 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i3                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa2a22_x4 : entity is TRUE;
end oa2a22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2a22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := ((i3_ipd) AND (i2_ipd)) OR ((i1_ipd) AND (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2a22_x4_VITAL of oa2a22_x4 is
   for VITAL
   end for;
end CFG_oa2a22_x4_VITAL;


----- CELL oa2ao222_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2ao222_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa2ao222_x2 : entity is TRUE;
end oa2ao222_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2ao222_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       (((i3_ipd) OR (i2_ipd)) AND (i4_ipd)) OR ((i1_ipd) AND (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2ao222_x2_VITAL of oa2ao222_x2 is
   for VITAL
   end for;
end CFG_oa2ao222_x2_VITAL;


----- CELL oa2ao222_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa2ao222_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa2ao222_x4 : entity is TRUE;
end oa2ao222_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa2ao222_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       (((i3_ipd) OR (i2_ipd)) AND (i4_ipd)) OR ((i1_ipd) AND (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa2ao222_x4_VITAL of oa2ao222_x4 is
   for VITAL
   end for;
end CFG_oa2ao222_x4_VITAL;


----- CELL oa3ao322_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa3ao322_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa3ao322_x2 : entity is TRUE;
end oa3ao322_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa3ao322_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       (((i4_ipd) OR (i3_ipd) OR (i5_ipd)) AND (i6_ipd)) OR ((i1_ipd) AND
         (i0_ipd) AND (i2_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_q, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa3ao322_x2_VITAL of oa3ao322_x2 is
   for VITAL
   end for;
end CFG_oa3ao322_x2_VITAL;


----- CELL oa3ao322_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa3ao322_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      i3                             :	in    STD_ULOGIC;
      i4                             :	in    STD_ULOGIC;
      i5                             :	in    STD_ULOGIC;
      i6                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa3ao322_x4 : entity is TRUE;
end oa3ao322_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa3ao322_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i3_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i4_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i5_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i6_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   VitalWireDelay (i3_ipd, i3, tipd_i3);
   VitalWireDelay (i4_ipd, i4, tipd_i4);
   VitalWireDelay (i5_ipd, i5, tipd_i5);
   VitalWireDelay (i6_ipd, i6, tipd_i6);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd, i3_ipd, i4_ipd, i5_ipd, i6_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd :=
       (((i4_ipd) OR (i3_ipd) OR (i5_ipd)) AND (i6_ipd)) OR ((i1_ipd) AND
         (i0_ipd) AND (i2_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE),
                 3 => (i3_ipd'last_event, tpd_i3_q, TRUE),
                 4 => (i4_ipd'last_event, tpd_i4_q, TRUE),
                 5 => (i5_ipd'last_event, tpd_i5_q, TRUE),
                 6 => (i6_ipd'last_event, tpd_i6_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa3ao322_x4_VITAL of oa3ao322_x4 is
   for VITAL
   end for;
end CFG_oa3ao322_x4_VITAL;


----- CELL oa22_x2 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa22_x2 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.390 ns, 0.555 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.488 ns, 0.525 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.438 ns, 0.454 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa22_x2 : entity is TRUE;
end oa22_x2;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa22_x2 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i2_ipd) OR ((i1_ipd) AND (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa22_x2_VITAL of oa22_x2 is
   for VITAL
   end for;
end CFG_oa22_x2_VITAL;


----- CELL oa22_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity oa22_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.511 ns, 0.677 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.615 ns, 0.650 ns);
      tpd_i2_q                       :	VitalDelayType01 := (0.523 ns, 0.571 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i2                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      i2                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of oa22_x4 : entity is TRUE;
end oa22_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of oa22_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i2_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (i2_ipd, i2, tipd_i2);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, i2_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i2_ipd) OR ((i1_ipd) AND (i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE),
                 2 => (i2_ipd'last_event, tpd_i2_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_oa22_x4_VITAL of oa22_x4 is
   for VITAL
   end for;
end CFG_oa22_x4_VITAL;


----- CELL on12_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity on12_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.111 ns, 0.234 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.314 ns, 0.291 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of on12_x1 : entity is TRUE;
end on12_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of on12_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_on12_x1_VITAL of on12_x1 is
   for VITAL
   end for;
end CFG_on12_x1_VITAL;


----- CELL on12_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity on12_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.474 ns, 0.499 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.491 ns, 0.394 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of on12_x4 : entity is TRUE;
end on12_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of on12_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) OR ((NOT i0_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_on12_x4_VITAL of on12_x4 is
   for VITAL
   end for;
end CFG_on12_x4_VITAL;


----- CELL one_x0 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity one_x0 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True);

   port(
      q                              :	out   STD_ULOGIC := '1');
attribute VITAL_LEVEL0 of one_x0 : entity is TRUE;
end one_x0;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of one_x0 is
   attribute VITAL_LEVEL0 of VITAL : architecture is TRUE;


begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   --  empty
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   q <= '1';


end VITAL;

configuration CFG_one_x0_VITAL of one_x0 is
   for VITAL
   end for;
end CFG_one_x0_VITAL;


----- CELL sff1_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity sff1_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_ck_q                       :	VitalDelayType01 := (0.500 ns, 0.500 ns);
      tsetup_i_ck                    :	VitalDelayType := 0.585 ns;
      thold_i_ck                     :	VitalDelayType := 0.000 ns;
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_ck                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      ck                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of sff1_x4 : entity is TRUE;
end sff1_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of sff1_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';
   SIGNAL ck_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   VitalWireDelay (ck_ipd, ck, tipd_ck);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd, ck_ipd)

   -- timing check results
   VARIABLE Tviol_i_ck_posedge	: STD_ULOGIC := '0';
   VARIABLE Tmkr_i_ck_posedge	: VitalTimingDataType := VitalTimingDataInit;

   -- functionality results
   VARIABLE Violation : STD_ULOGIC := '0';
   VARIABLE PrevData_q : STD_LOGIC_VECTOR(0 to 2);
   VARIABLE i_delayed : STD_ULOGIC := 'X';
   VARIABLE ck_delayed : STD_ULOGIC := 'X';
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      ------------------------
      --  Timing Check Section
      ------------------------
      if (TimingChecksOn) then
         VitalSetupHoldCheck (
          Violation               => Tviol_i_ck_posedge,
          TimingData              => Tmkr_i_ck_posedge,
          TestSignal              => i_ipd,
          TestSignalName          => "i",
          TestDelay               => 0 ns,
          RefSignal               => ck_ipd,
          RefSignalName          => "ck",
          RefDelay                => 0 ns,
          SetupHigh               => tsetup_i_ck,
          SetupLow                => tsetup_i_ck,
          HoldHigh                => thold_i_ck,
          HoldLow                 => thold_i_ck,
          CheckEnabled            => 
                           TRUE,
          RefTransition           => 'R',
          HeaderMsg               => InstancePath & "/sff1_x4",
          Xon                     => Xon,
          MsgOn                   => MsgOn,
          MsgSeverity             => WARNING);
      end if;

      -------------------------
      --  Functionality Section
      -------------------------
      Violation := Tviol_i_ck_posedge;
      VitalStateTable(
        Result => q_zd,
        PreviousDataIn => PrevData_q,
        StateTable => sff1_x4_q_tab,
        DataIn => (
               ck_delayed, i_delayed, ck_ipd));
      q_zd := Violation XOR q_zd;
      i_delayed := i_ipd;
      ck_delayed := ck_ipd;

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (ck_ipd'last_event, tpd_ck_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_sff1_x4_VITAL of sff1_x4 is
   for VITAL
   end for;
end CFG_sff1_x4_VITAL;


----- CELL sff2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity sff2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
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

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      ck                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of sff2_x4 : entity is TRUE;
end sff2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of sff2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';
   SIGNAL ck_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   VitalWireDelay (ck_ipd, ck, tipd_ck);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd, cmd_ipd, ck_ipd)

   -- timing check results
   VARIABLE Tviol_i0_ck_posedge	: STD_ULOGIC := '0';
   VARIABLE Tmkr_i0_ck_posedge	: VitalTimingDataType := VitalTimingDataInit;
   VARIABLE Tviol_i1_ck_posedge	: STD_ULOGIC := '0';
   VARIABLE Tmkr_i1_ck_posedge	: VitalTimingDataType := VitalTimingDataInit;
   VARIABLE Tviol_cmd_ck_posedge	: STD_ULOGIC := '0';
   VARIABLE Tmkr_cmd_ck_posedge	: VitalTimingDataType := VitalTimingDataInit;

   -- functionality results
   VARIABLE Violation : STD_ULOGIC := '0';
   VARIABLE PrevData_q : STD_LOGIC_VECTOR(0 to 4);
   VARIABLE i0_delayed : STD_ULOGIC := 'X';
   VARIABLE i1_delayed : STD_ULOGIC := 'X';
   VARIABLE cmd_delayed : STD_ULOGIC := 'X';
   VARIABLE ck_delayed : STD_ULOGIC := 'X';
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      ------------------------
      --  Timing Check Section
      ------------------------
      if (TimingChecksOn) then
         VitalSetupHoldCheck (
          Violation               => Tviol_i0_ck_posedge,
          TimingData              => Tmkr_i0_ck_posedge,
          TestSignal              => i0_ipd,
          TestSignalName          => "i0",
          TestDelay               => 0 ns,
          RefSignal               => ck_ipd,
          RefSignalName          => "ck",
          RefDelay                => 0 ns,
          SetupHigh               => tsetup_i0_ck,
          SetupLow                => tsetup_i0_ck,
          HoldHigh                => thold_i0_ck,
          HoldLow                 => thold_i0_ck,
          CheckEnabled            => 
                           TRUE,
          RefTransition           => 'R',
          HeaderMsg               => InstancePath & "/sff2_x4",
          Xon                     => Xon,
          MsgOn                   => MsgOn,
          MsgSeverity             => WARNING);
         VitalSetupHoldCheck (
          Violation               => Tviol_i1_ck_posedge,
          TimingData              => Tmkr_i1_ck_posedge,
          TestSignal              => i1_ipd,
          TestSignalName          => "i1",
          TestDelay               => 0 ns,
          RefSignal               => ck_ipd,
          RefSignalName          => "ck",
          RefDelay                => 0 ns,
          SetupHigh               => tsetup_i1_ck,
          SetupLow                => tsetup_i1_ck,
          HoldHigh                => thold_i1_ck,
          HoldLow                 => thold_i1_ck,
          CheckEnabled            => 
                           TRUE,
          RefTransition           => 'R',
          HeaderMsg               => InstancePath & "/sff2_x4",
          Xon                     => Xon,
          MsgOn                   => MsgOn,
          MsgSeverity             => WARNING);
         VitalSetupHoldCheck (
          Violation               => Tviol_cmd_ck_posedge,
          TimingData              => Tmkr_cmd_ck_posedge,
          TestSignal              => cmd_ipd,
          TestSignalName          => "cmd",
          TestDelay               => 0 ns,
          RefSignal               => ck_ipd,
          RefSignalName          => "ck",
          RefDelay                => 0 ns,
          SetupHigh               => tsetup_cmd_ck,
          SetupLow                => tsetup_cmd_ck,
          HoldHigh                => thold_cmd_ck,
          HoldLow                 => thold_cmd_ck,
          CheckEnabled            => 
                           TRUE,
          RefTransition           => 'R',
          HeaderMsg               => InstancePath & "/sff2_x4",
          Xon                     => Xon,
          MsgOn                   => MsgOn,
          MsgSeverity             => WARNING);
      end if;

      -------------------------
      --  Functionality Section
      -------------------------
      Violation := Tviol_i0_ck_posedge or Tviol_i1_ck_posedge or Tviol_cmd_ck_posedge;
      VitalStateTable(
        Result => q_zd,
        PreviousDataIn => PrevData_q,
        StateTable => sff2_x4_q_tab,
        DataIn => (
               ck_delayed, i1_delayed, i0_delayed, cmd_delayed, ck_ipd));
      q_zd := Violation XOR q_zd;
      i0_delayed := i0_ipd;
      i1_delayed := i1_ipd;
      cmd_delayed := cmd_ipd;
      ck_delayed := ck_ipd;

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (ck_ipd'last_event, tpd_ck_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_sff2_x4_VITAL of sff2_x4 is
   for VITAL
   end for;
end CFG_sff2_x4_VITAL;


----- CELL ts_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity ts_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_q                      :	VitalDelayType01z := 
               (0.492 ns, 0.409 ns, 0.492 ns, 0.492 ns, 0.409 ns, 0.409 ns);
      tpd_i_q                        :	VitalDelayType01 := (0.475 ns, 0.444 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of ts_x4 : entity is TRUE;
end ts_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of ts_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd, cmd_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := VitalBUFIF0 (data => i_ipd,
              enable => (NOT cmd_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01Z (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (cmd_ipd'last_event, VitalExtendToFillDelay(tpd_cmd_q), TRUE),
                 1 => (i_ipd'last_event, VitalExtendToFillDelay(tpd_i_q), TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING,
       OutputMap => "UX01ZWLH-");

end process;

end VITAL;

configuration CFG_ts_x4_VITAL of ts_x4 is
   for VITAL
   end for;
end CFG_ts_x4_VITAL;


----- CELL ts_x8 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity ts_x8 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_cmd_q                      :	VitalDelayType01z := 
               (0.626 ns, 0.466 ns, 0.626 ns, 0.626 ns, 0.466 ns, 0.466 ns);
      tpd_i_q                        :	VitalDelayType01 := (0.613 ns, 0.569 ns);
      tipd_i                         :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_cmd                       :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i                              :	in    STD_ULOGIC;
      cmd                            :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of ts_x8 : entity is TRUE;
end ts_x8;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of ts_x8 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i_ipd	 : STD_ULOGIC := 'X';
   SIGNAL cmd_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i_ipd, i, tipd_i);
   VitalWireDelay (cmd_ipd, cmd, tipd_cmd);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i_ipd, cmd_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := VitalBUFIF0 (data => i_ipd,
              enable => (NOT cmd_ipd));

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01Z (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (cmd_ipd'last_event, VitalExtendToFillDelay(tpd_cmd_q), TRUE),
                 1 => (i_ipd'last_event, VitalExtendToFillDelay(tpd_i_q), TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING,
       OutputMap => "UX01ZWLH-");

end process;

end VITAL;

configuration CFG_ts_x8_VITAL of ts_x8 is
   for VITAL
   end for;
end CFG_ts_x8_VITAL;


----- CELL xr2_x1 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity xr2_x1 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.292 ns, 0.293 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.377 ns, 0.261 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of xr2_x1 : entity is TRUE;
end xr2_x1;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of xr2_x1 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) XOR (i0_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_xr2_x1_VITAL of xr2_x1 is
   for VITAL
   end for;
end CFG_xr2_x1_VITAL;


----- CELL xr2_x4 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity xr2_x4 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True;
      tpd_i0_q                       :	VitalDelayType01 := (0.521 ns, 0.560 ns);
      tpd_i1_q                       :	VitalDelayType01 := (0.541 ns, 0.657 ns);
      tipd_i0                        :	VitalDelayType01 := (0.000 ns, 0.000 ns);
      tipd_i1                        :	VitalDelayType01 := (0.000 ns, 0.000 ns));

   port(
      i0                             :	in    STD_ULOGIC;
      i1                             :	in    STD_ULOGIC;
      q                              :	out   STD_ULOGIC);
attribute VITAL_LEVEL0 of xr2_x4 : entity is TRUE;
end xr2_x4;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of xr2_x4 is
   attribute VITAL_LEVEL1 of VITAL : architecture is TRUE;

   SIGNAL i0_ipd	 : STD_ULOGIC := 'X';
   SIGNAL i1_ipd	 : STD_ULOGIC := 'X';

begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   VitalWireDelay (i0_ipd, i0, tipd_i0);
   VitalWireDelay (i1_ipd, i1, tipd_i1);
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   VITALBehavior : process (i0_ipd, i1_ipd)


   -- functionality results
   VARIABLE Results : STD_LOGIC_VECTOR(1 to 1) := (others => 'X');
   ALIAS q_zd : STD_LOGIC is Results(1);

   -- output glitch detection variables
   VARIABLE q_GlitchData	: VitalGlitchDataType;

   begin

      -------------------------
      --  Functionality Section
      -------------------------
      q_zd := (i1_ipd) XOR (i0_ipd);

      ----------------------
      --  Path Delay Section
      ----------------------
      VitalPathDelay01 (
       OutSignal => q,
       GlitchData => q_GlitchData,
       OutSignalName => "q",
       OutTemp => q_zd,
       Paths => (0 => (i0_ipd'last_event, tpd_i0_q, TRUE),
                 1 => (i1_ipd'last_event, tpd_i1_q, TRUE)),
       Mode => OnDetect,
       Xon => Xon,
       MsgOn => MsgOn,
       MsgSeverity => WARNING);

end process;

end VITAL;

configuration CFG_xr2_x4_VITAL of xr2_x4 is
   for VITAL
   end for;
end CFG_xr2_x4_VITAL;


----- CELL zero_x0 -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;
library IEEE;
use IEEE.VITAL_Timing.all;


-- entity declaration --
entity zero_x0 is
   generic(
      TimingChecksOn: Boolean := True;
      InstancePath: STRING := "*";
      Xon: Boolean := False;
      MsgOn: Boolean := True);

   port(
      nq                             :	out   STD_ULOGIC := '0');
attribute VITAL_LEVEL0 of zero_x0 : entity is TRUE;
end zero_x0;

-- architecture body --
library IEEE;
use IEEE.VITAL_Primitives.all;
library sxlib;
use sxlib.VTABLES.all;
architecture VITAL of zero_x0 is
   attribute VITAL_LEVEL0 of VITAL : architecture is TRUE;


begin

   ---------------------
   --  INPUT PATH DELAYs
   ---------------------
   WireDelay : block
   begin
   --  empty
   end block;
   --------------------
   --  BEHAVIOR SECTION
   --------------------
   nq <= '0';


end VITAL;

configuration CFG_zero_x0_VITAL of zero_x0 is
   for VITAL
   end for;
end CFG_zero_x0_VITAL;


---- end of library ----
