ENTITY oa2a2a2a24_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 4000;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 13;
  CONSTANT cin_i2	 : NATURAL := 14;
  CONSTANT cin_i3	 : NATURAL := 13;
  CONSTANT cin_i4	 : NATURAL := 13;
  CONSTANT cin_i5	 : NATURAL := 13;
  CONSTANT cin_i6	 : NATURAL := 14;
  CONSTANT cin_i7	 : NATURAL := 14;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rdown_i3_q	 : NATURAL := 810;
  CONSTANT rdown_i4_q	 : NATURAL := 810;
  CONSTANT rdown_i5_q	 : NATURAL := 810;
  CONSTANT rdown_i6_q	 : NATURAL := 810;
  CONSTANT rdown_i7_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT rup_i4_q	 : NATURAL := 890;
  CONSTANT rup_i5_q	 : NATURAL := 890;
  CONSTANT rup_i6_q	 : NATURAL := 890;
  CONSTANT rup_i7_q	 : NATURAL := 890;
  CONSTANT tphh_i7_q	 : NATURAL := 399;
  CONSTANT tphh_i6_q	 : NATURAL := 487;
  CONSTANT tphh_i5_q	 : NATURAL := 515;
  CONSTANT tphh_i4_q	 : NATURAL := 619;
  CONSTANT tphh_i2_q	 : NATURAL := 726;
  CONSTANT tphh_i0_q	 : NATURAL := 823;
  CONSTANT tpll_i1_q	 : NATURAL := 835;
  CONSTANT tpll_i6_q	 : NATURAL := 845;
  CONSTANT tphh_i3_q	 : NATURAL := 851;
  CONSTANT tpll_i0_q	 : NATURAL := 879;
  CONSTANT tpll_i3_q	 : NATURAL := 895;
  CONSTANT tpll_i7_q	 : NATURAL := 895;
  CONSTANT tpll_i4_q	 : NATURAL := 902;
  CONSTANT tpll_i2_q	 : NATURAL := 940;
  CONSTANT tpll_i5_q	 : NATURAL := 949;
  CONSTANT tphh_i1_q	 : NATURAL := 955;
  CONSTANT transistors	 : NATURAL := 20
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  i5	 : in  BIT;
  i6	 : in  BIT;
  i7	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END oa2a2a2a24_x4;

ARCHITECTURE behaviour_data_flow OF oa2a2a2a24_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa2a2a2a24_x4"
  SEVERITY WARNING;
  q <= ((((i0 and i1) or (i2 and i3)) or (i4 and i5)) or (i6 and i7)) after 1600 ps;
END;
