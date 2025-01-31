ENTITY ao22_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT tpll_i2_q	 : NATURAL := 505;
  CONSTANT tphh_i2_q	 : NATURAL := 526;
  CONSTANT tpll_i0_q	 : NATURAL := 552;
  CONSTANT tphh_i1_q	 : NATURAL := 615;
  CONSTANT tpll_i1_q	 : NATURAL := 647;
  CONSTANT tphh_i0_q	 : NATURAL := 674;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END ao22_x4;

ARCHITECTURE behaviour_data_flow OF ao22_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on ao22_x4"
  SEVERITY WARNING;
  q <= ((i0 or i1) and i2) after 1300 ps;
END;
