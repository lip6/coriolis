ENTITY on12_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 2850;
  CONSTANT rdown_i1_q	 : NATURAL := 2850;
  CONSTANT rup_i0_q	 : NATURAL := 3720;
  CONSTANT rup_i1_q	 : NATURAL := 3720;
  CONSTANT tphl_i0_q	 : NATURAL := 111;
  CONSTANT tplh_i0_q	 : NATURAL := 234;
  CONSTANT tpll_i1_q	 : NATURAL := 291;
  CONSTANT tphh_i1_q	 : NATURAL := 314;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END on12_x1;

ARCHITECTURE behaviour_data_flow OF on12_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on on12_x1"
  SEVERITY WARNING;
  q <= (not (i0) or i1) after 900 ps;
END;
