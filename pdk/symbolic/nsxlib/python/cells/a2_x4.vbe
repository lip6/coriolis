ENTITY a2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT tphh_i1_q	 : NATURAL := 269;
  CONSTANT tphh_i0_q	 : NATURAL := 338;
  CONSTANT tpll_i0_q	 : NATURAL := 476;
  CONSTANT tpll_i1_q	 : NATURAL := 518;
  CONSTANT transistors	 : NATURAL := 8
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END a2_x4;

ARCHITECTURE behaviour_data_flow OF a2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on a2_x4"
  SEVERITY WARNING;
  q <= (i0 and i1) after 1100 ps;
END;
