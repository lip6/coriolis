ENTITY oa3ao322_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2750;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT cin_i3	 : NATURAL := 9;
  CONSTANT cin_i4	 : NATURAL := 9;
  CONSTANT cin_i5	 : NATURAL := 9;
  CONSTANT cin_i6	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rdown_i3_q	 : NATURAL := 1620;
  CONSTANT rdown_i4_q	 : NATURAL := 1620;
  CONSTANT rdown_i5_q	 : NATURAL := 1620;
  CONSTANT rdown_i6_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT rup_i3_q	 : NATURAL := 1790;
  CONSTANT rup_i4_q	 : NATURAL := 1790;
  CONSTANT rup_i5_q	 : NATURAL := 1790;
  CONSTANT rup_i6_q	 : NATURAL := 1790;
  CONSTANT tpll_i6_q	 : NATURAL := 540;
  CONSTANT tphh_i3_q	 : NATURAL := 560;
  CONSTANT tphh_i6_q	 : NATURAL := 563;
  CONSTANT tphh_i0_q	 : NATURAL := 638;
  CONSTANT tphh_i4_q	 : NATURAL := 649;
  CONSTANT tpll_i2_q	 : NATURAL := 707;
  CONSTANT tphh_i5_q	 : NATURAL := 734;
  CONSTANT tpll_i5_q	 : NATURAL := 734;
  CONSTANT tphh_i1_q	 : NATURAL := 735;
  CONSTANT tpll_i4_q	 : NATURAL := 760;
  CONSTANT tpll_i1_q	 : NATURAL := 764;
  CONSTANT tpll_i3_q	 : NATURAL := 765;
  CONSTANT tphh_i2_q	 : NATURAL := 806;
  CONSTANT tpll_i0_q	 : NATURAL := 820;
  CONSTANT transistors	 : NATURAL := 16
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
END oa3ao322_x2;

ARCHITECTURE behaviour_data_flow OF oa3ao322_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa3ao322_x2"
  SEVERITY WARNING;
  q <= (((i0 and i1) and i2) or (i6 and ((i3 or i4) or i5))) after 1400 ps;
END;
