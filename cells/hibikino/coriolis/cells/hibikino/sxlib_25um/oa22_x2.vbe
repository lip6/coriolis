ENTITY oa22_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT tphh_i0_q	 : NATURAL := 390;
  CONSTANT tphh_i2_q	 : NATURAL := 438;
  CONSTANT tpll_i2_q	 : NATURAL := 454;
  CONSTANT tphh_i1_q	 : NATURAL := 488;
  CONSTANT tpll_i1_q	 : NATURAL := 525;
  CONSTANT tpll_i0_q	 : NATURAL := 555;
  CONSTANT transistors	 : NATURAL := 8
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END oa22_x2;

ARCHITECTURE behaviour_data_flow OF oa22_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa22_x2"
  SEVERITY WARNING;
  q <= ((i0 and i1) or i2) after 1200 ps;
END;
