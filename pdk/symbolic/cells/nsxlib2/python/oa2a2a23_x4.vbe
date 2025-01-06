ENTITY oa2a2a23_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3250;
  CONSTANT cin_i0	 : NATURAL := 13;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT cin_i2	 : NATURAL := 14;
  CONSTANT cin_i3	 : NATURAL := 14;
  CONSTANT cin_i4	 : NATURAL := 14;
  CONSTANT cin_i5	 : NATURAL := 14;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rdown_i3_q	 : NATURAL := 810;
  CONSTANT rdown_i4_q	 : NATURAL := 810;
  CONSTANT rdown_i5_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT rup_i4_q	 : NATURAL := 890;
  CONSTANT rup_i5_q	 : NATURAL := 890;
  CONSTANT tphh_i5_q	 : NATURAL := 379;
  CONSTANT tphh_i4_q	 : NATURAL := 464;
  CONSTANT tphh_i2_q	 : NATURAL := 493;
  CONSTANT tphh_i3_q	 : NATURAL := 594;
  CONSTANT tpll_i1_q	 : NATURAL := 613;
  CONSTANT tpll_i0_q	 : NATURAL := 648;
  CONSTANT tpll_i4_q	 : NATURAL := 673;
  CONSTANT tpll_i3_q	 : NATURAL := 677;
  CONSTANT tphh_i0_q	 : NATURAL := 699;
  CONSTANT tpll_i5_q	 : NATURAL := 714;
  CONSTANT tpll_i2_q	 : NATURAL := 715;
  CONSTANT tphh_i1_q	 : NATURAL := 822;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  i5	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END oa2a2a23_x4;

ARCHITECTURE behaviour_data_flow OF oa2a2a23_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa2a2a23_x4"
  SEVERITY WARNING;
  q <= (((i0 and i1) or (i2 and i3)) or (i4 and i5)) after 1400 ps;
END;
