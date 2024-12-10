ENTITY oa22_x4 IS
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
  CONSTANT tphh_i0_q	 : NATURAL := 511;
  CONSTANT tphh_i2_q	 : NATURAL := 523;
  CONSTANT tpll_i2_q	 : NATURAL := 571;
  CONSTANT tphh_i1_q	 : NATURAL := 615;
  CONSTANT tpll_i1_q	 : NATURAL := 650;
  CONSTANT tpll_i0_q	 : NATURAL := 677;
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
END oa22_x4;

ARCHITECTURE behaviour_data_flow OF oa22_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa22_x4"
  SEVERITY WARNING;
  q <= ((i0 and i1) or i2) after 1300 ps;
END;
