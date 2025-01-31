ENTITY a4_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 10;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT cin_i3	 : NATURAL := 10;
  CONSTANT rdown_i0_q	 : NATURAL := 540;
  CONSTANT rdown_i1_q	 : NATURAL := 540;
  CONSTANT rdown_i2_q	 : NATURAL := 540;
  CONSTANT rdown_i3_q	 : NATURAL := 540;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT tphh_i0_q	 : NATURAL := 505;
  CONSTANT tpll_i3_q	 : NATURAL := 538;
  CONSTANT tpll_i2_q	 : NATURAL := 576;
  CONSTANT tphh_i1_q	 : NATURAL := 578;
  CONSTANT tpll_i1_q	 : NATURAL := 614;
  CONSTANT tphh_i2_q	 : NATURAL := 627;
  CONSTANT tpll_i0_q	 : NATURAL := 650;
  CONSTANT tphh_i3_q	 : NATURAL := 661;
  CONSTANT transistors	 : NATURAL := 13
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
END a4_x4;

ARCHITECTURE behaviour_data_flow OF a4_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on a4_x4"
  SEVERITY WARNING;
  q <= (((i0 and i1) and i2) and i3) after 1300 ps;
END;
