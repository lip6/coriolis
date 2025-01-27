ENTITY o4_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 12;
  CONSTANT cin_i2	 : NATURAL := 12;
  CONSTANT cin_i3	 : NATURAL := 12;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rdown_i3_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT tphh_i1_q	 : NATURAL := 492;
  CONSTANT tpll_i3_q	 : NATURAL := 536;
  CONSTANT tphh_i0_q	 : NATURAL := 574;
  CONSTANT tpll_i2_q	 : NATURAL := 611;
  CONSTANT tpll_i0_q	 : NATURAL := 638;
  CONSTANT tphh_i2_q	 : NATURAL := 649;
  CONSTANT tpll_i1_q	 : NATURAL := 650;
  CONSTANT tphh_i3_q	 : NATURAL := 721;
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
END o4_x4;

ARCHITECTURE behaviour_data_flow OF o4_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on o4_x4"
  SEVERITY WARNING;
  q <= (((i0 or i1) or i2) or i3) after 1300 ps;
END;
