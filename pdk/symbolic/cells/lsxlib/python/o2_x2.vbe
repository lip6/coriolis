ENTITY o2_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 10;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT tpll_i0_q	 : NATURAL := 310;
  CONSTANT tphh_i1_q	 : NATURAL := 335;
  CONSTANT tpll_i1_q	 : NATURAL := 364;
  CONSTANT tphh_i0_q	 : NATURAL := 406;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END o2_x2;

ARCHITECTURE behaviour_data_flow OF o2_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on o2_x2"
  SEVERITY WARNING;
  q <= (i0 or i1) after 1000 ps;
END;
