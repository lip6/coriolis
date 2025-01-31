ENTITY o4_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 10;
  CONSTANT cin_i2	 : NATURAL := 10;
  CONSTANT cin_i3	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rdown_i3_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT rup_i3_q	 : NATURAL := 1790;
  CONSTANT tphh_i3_q	 : NATURAL := 378;
  CONSTANT tphh_i1_q	 : NATURAL := 446;
  CONSTANT tphh_i0_q	 : NATURAL := 508;
  CONSTANT tpll_i2_q	 : NATURAL := 531;
  CONSTANT tphh_i2_q	 : NATURAL := 567;
  CONSTANT tpll_i0_q	 : NATURAL := 601;
  CONSTANT tpll_i3_q	 : NATURAL := 626;
  CONSTANT tpll_i1_q	 : NATURAL := 631;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END o4_x2;

ARCHITECTURE behaviour_data_flow OF o4_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on o4_x2"
  SEVERITY WARNING;
  q <= (((i0 or i1) or i2) or i3) after 1200 ps;
END;
