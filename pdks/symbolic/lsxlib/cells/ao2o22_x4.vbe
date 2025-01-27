ENTITY ao2o22_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2500;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT cin_i3	 : NATURAL := 8;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rdown_i3_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT tphh_i2_q	 : NATURAL := 554;
  CONSTANT tpll_i0_q	 : NATURAL := 569;
  CONSTANT tphh_i3_q	 : NATURAL := 606;
  CONSTANT tphh_i1_q	 : NATURAL := 637;
  CONSTANT tpll_i3_q	 : NATURAL := 639;
  CONSTANT tpll_i1_q	 : NATURAL := 666;
  CONSTANT tphh_i0_q	 : NATURAL := 696;
  CONSTANT tpll_i2_q	 : NATURAL := 744;
  CONSTANT transistors	 : NATURAL := 12
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
END ao2o22_x4;

ARCHITECTURE behaviour_data_flow OF ao2o22_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on ao2o22_x4"
  SEVERITY WARNING;
  q <= ((i0 or i1) and (i2 or i3)) after 1300 ps;
END;
