ENTITY an12_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 3640;
  CONSTANT rdown_i1_q	 : NATURAL := 3640;
  CONSTANT rup_i0_q	 : NATURAL := 3210;
  CONSTANT rup_i1_q	 : NATURAL := 3210;
  CONSTANT tplh_i0_q	 : NATURAL := 168;
  CONSTANT tphl_i0_q	 : NATURAL := 200;
  CONSTANT tphh_i1_q	 : NATURAL := 285;
  CONSTANT tpll_i1_q	 : NATURAL := 405;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END an12_x1;

ARCHITECTURE behaviour_data_flow OF an12_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on an12_x1"
  SEVERITY WARNING;
  q <= (not (i0) and i1) after 1000 ps;
END;
