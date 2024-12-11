ENTITY a3_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT tphh_i2_q	 : NATURAL := 356;
  CONSTANT tphh_i1_q	 : NATURAL := 428;
  CONSTANT tphh_i0_q	 : NATURAL := 478;
  CONSTANT tpll_i0_q	 : NATURAL := 514;
  CONSTANT tpll_i1_q	 : NATURAL := 554;
  CONSTANT tpll_i2_q	 : NATURAL := 592;
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
END a3_x4;

ARCHITECTURE behaviour_data_flow OF a3_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on a3_x4"
  SEVERITY WARNING;
  q <= ((i0 and i1) and i2) after 1200 ps;
END;
