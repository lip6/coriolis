ENTITY oa3ao322_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3000;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT cin_i3	 : NATURAL := 9;
  CONSTANT cin_i4	 : NATURAL := 9;
  CONSTANT cin_i5	 : NATURAL := 9;
  CONSTANT cin_i6	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rdown_i3_q	 : NATURAL := 810;
  CONSTANT rdown_i4_q	 : NATURAL := 810;
  CONSTANT rdown_i5_q	 : NATURAL := 810;
  CONSTANT rdown_i6_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT rup_i4_q	 : NATURAL := 890;
  CONSTANT rup_i5_q	 : NATURAL := 890;
  CONSTANT rup_i6_q	 : NATURAL := 890;
  CONSTANT tpll_i6_q	 : NATURAL := 651;
  CONSTANT tphh_i3_q	 : NATURAL := 673;
  CONSTANT tphh_i6_q	 : NATURAL := 684;
  CONSTANT tphh_i0_q	 : NATURAL := 717;
  CONSTANT tphh_i4_q	 : NATURAL := 758;
  CONSTANT tphh_i1_q	 : NATURAL := 818;
  CONSTANT tpll_i2_q	 : NATURAL := 834;
  CONSTANT tphh_i5_q	 : NATURAL := 839;
  CONSTANT tpll_i5_q	 : NATURAL := 865;
  CONSTANT tpll_i1_q	 : NATURAL := 890;
  CONSTANT tphh_i2_q	 : NATURAL := 894;
  CONSTANT tpll_i4_q	 : NATURAL := 896;
  CONSTANT tpll_i3_q	 : NATURAL := 898;
  CONSTANT tpll_i0_q	 : NATURAL := 946;
  CONSTANT transistors	 : NATURAL := 18
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  i5	 : in  BIT;
  i6	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END oa3ao322_x4;

ARCHITECTURE behaviour_data_flow OF oa3ao322_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa3ao322_x4"
  SEVERITY WARNING;
  q <= (((i0 and i1) and i2) or (i6 and ((i3 or i4) or i5))) after 1500 ps;
END;
