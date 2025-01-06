ENTITY oa2a2a2a24_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3750;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 13;
  CONSTANT cin_i2	 : NATURAL := 14;
  CONSTANT cin_i3	 : NATURAL := 13;
  CONSTANT cin_i4	 : NATURAL := 13;
  CONSTANT cin_i5	 : NATURAL := 13;
  CONSTANT cin_i6	 : NATURAL := 14;
  CONSTANT cin_i7	 : NATURAL := 14;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rdown_i3_q	 : NATURAL := 1620;
  CONSTANT rdown_i4_q	 : NATURAL := 1620;
  CONSTANT rdown_i5_q	 : NATURAL := 1620;
  CONSTANT rdown_i6_q	 : NATURAL := 1620;
  CONSTANT rdown_i7_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT rup_i3_q	 : NATURAL := 1790;
  CONSTANT rup_i4_q	 : NATURAL := 1790;
  CONSTANT rup_i5_q	 : NATURAL := 1790;
  CONSTANT rup_i6_q	 : NATURAL := 1790;
  CONSTANT rup_i7_q	 : NATURAL := 1790;
  CONSTANT tphh_i7_q	 : NATURAL := 346;
  CONSTANT tphh_i6_q	 : NATURAL := 426;
  CONSTANT tphh_i5_q	 : NATURAL := 467;
  CONSTANT tphh_i4_q	 : NATURAL := 565;
  CONSTANT tphh_i2_q	 : NATURAL := 682;
  CONSTANT tpll_i6_q	 : NATURAL := 748;
  CONSTANT tpll_i1_q	 : NATURAL := 753;
  CONSTANT tphh_i0_q	 : NATURAL := 780;
  CONSTANT tpll_i0_q	 : NATURAL := 797;
  CONSTANT tpll_i7_q	 : NATURAL := 800;
  CONSTANT tphh_i3_q	 : NATURAL := 803;
  CONSTANT tpll_i3_q	 : NATURAL := 810;
  CONSTANT tpll_i4_q	 : NATURAL := 813;
  CONSTANT tpll_i2_q	 : NATURAL := 856;
  CONSTANT tpll_i5_q	 : NATURAL := 861;
  CONSTANT tphh_i1_q	 : NATURAL := 909;
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
  i7	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END oa2a2a2a24_x2;

ARCHITECTURE behaviour_data_flow OF oa2a2a2a24_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa2a2a2a24_x2"
  SEVERITY WARNING;
  q <= ((((i0 and i1) or (i2 and i3)) or (i4 and i5)) or (i6 and i7)) after 1500 ps;
END;
