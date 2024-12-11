ENTITY ao2o22_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2250;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT cin_i3	 : NATURAL := 8;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rdown_i3_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT rup_i3_q	 : NATURAL := 1790;
  CONSTANT tphh_i2_q	 : NATURAL := 432;
  CONSTANT tpll_i0_q	 : NATURAL := 451;
  CONSTANT tphh_i3_q	 : NATURAL := 488;
  CONSTANT tphh_i1_q	 : NATURAL := 508;
  CONSTANT tpll_i3_q	 : NATURAL := 526;
  CONSTANT tpll_i1_q	 : NATURAL := 542;
  CONSTANT tphh_i0_q	 : NATURAL := 572;
  CONSTANT tpll_i2_q	 : NATURAL := 627;
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
END ao2o22_x2;

ARCHITECTURE behaviour_data_flow OF ao2o22_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on ao2o22_x2"
  SEVERITY WARNING;
  q <= ((i0 or i1) and (i2 or i3)) after 1200 ps;
END;
