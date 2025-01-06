ENTITY a2_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT tphh_i1_q	 : NATURAL := 203;
  CONSTANT tphh_i0_q	 : NATURAL := 261;
  CONSTANT tpll_i0_q	 : NATURAL := 388;
  CONSTANT tpll_i1_q	 : NATURAL := 434;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END a2_x2;

ARCHITECTURE behaviour_data_flow OF a2_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on a2_x2"
  SEVERITY WARNING;
  q <= (i0 and i1) after 1000 ps;
END;
