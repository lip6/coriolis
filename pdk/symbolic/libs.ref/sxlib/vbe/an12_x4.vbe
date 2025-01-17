ENTITY an12_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT tphh_i1_q	 : NATURAL := 269;
  CONSTANT tphl_i0_q	 : NATURAL := 461;
  CONSTANT tplh_i0_q	 : NATURAL := 471;
  CONSTANT tpll_i1_q	 : NATURAL := 518;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END an12_x4;

ARCHITECTURE behaviour_data_flow OF an12_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on an12_x4"
  SEVERITY WARNING;
  q <= (not (i0) and i1) after 1100 ps;
END;
