ENTITY oa2a22_x2 IS
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
  CONSTANT tphh_i0_q	 : NATURAL := 403;
  CONSTANT tpll_i2_q	 : NATURAL := 487;
  CONSTANT tphh_i1_q	 : NATURAL := 495;
  CONSTANT tpll_i3_q	 : NATURAL := 512;
  CONSTANT tpll_i1_q	 : NATURAL := 534;
  CONSTANT tphh_i3_q	 : NATURAL := 537;
  CONSTANT tpll_i0_q	 : NATURAL := 564;
  CONSTANT tphh_i2_q	 : NATURAL := 646;
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
END oa2a22_x2;

ARCHITECTURE behaviour_data_flow OF oa2a22_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa2a22_x2"
  SEVERITY WARNING;
  q <= ((i0 and i1) or (i2 and i3)) after 1200 ps;
END;
