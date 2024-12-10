ENTITY noa3ao322_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2250;
  CONSTANT cin_i0	 : NATURAL := 13;
  CONSTANT cin_i1	 : NATURAL := 13;
  CONSTANT cin_i2	 : NATURAL := 13;
  CONSTANT cin_i3	 : NATURAL := 13;
  CONSTANT cin_i4	 : NATURAL := 13;
  CONSTANT cin_i5	 : NATURAL := 13;
  CONSTANT cin_i6	 : NATURAL := 13;
  CONSTANT rdown_i0_nq	 : NATURAL := 3370;
  CONSTANT rdown_i1_nq	 : NATURAL := 3370;
  CONSTANT rdown_i2_nq	 : NATURAL := 3370;
  CONSTANT rdown_i3_nq	 : NATURAL := 3210;
  CONSTANT rdown_i4_nq	 : NATURAL := 3210;
  CONSTANT rdown_i5_nq	 : NATURAL := 3210;
  CONSTANT rdown_i6_nq	 : NATURAL := 3210;
  CONSTANT rup_i0_nq	 : NATURAL := 6700;
  CONSTANT rup_i1_nq	 : NATURAL := 6700;
  CONSTANT rup_i2_nq	 : NATURAL := 6700;
  CONSTANT rup_i3_nq	 : NATURAL := 6700;
  CONSTANT rup_i4_nq	 : NATURAL := 6700;
  CONSTANT rup_i5_nq	 : NATURAL := 6700;
  CONSTANT rup_i6_nq	 : NATURAL := 3690;
  CONSTANT tphl_i3_nq	 : NATURAL := 196;
  CONSTANT tphl_i6_nq	 : NATURAL := 246;
  CONSTANT tphl_i4_nq	 : NATURAL := 264;
  CONSTANT tplh_i6_nq	 : NATURAL := 311;
  CONSTANT tphl_i5_nq	 : NATURAL := 328;
  CONSTANT tphl_i0_nq	 : NATURAL := 396;
  CONSTANT tphl_i1_nq	 : NATURAL := 486;
  CONSTANT tplh_i2_nq	 : NATURAL := 488;
  CONSTANT tphl_i2_nq	 : NATURAL := 546;
  CONSTANT tplh_i1_nq	 : NATURAL := 552;
  CONSTANT tplh_i5_nq	 : NATURAL := 581;
  CONSTANT tplh_i3_nq	 : NATURAL := 599;
  CONSTANT tplh_i4_nq	 : NATURAL := 608;
  CONSTANT tplh_i0_nq	 : NATURAL := 616;
  CONSTANT transistors	 : NATURAL := 14
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  i5	 : in  BIT;
  i6	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa3ao322_x1;

ARCHITECTURE behaviour_data_flow OF noa3ao322_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa3ao322_x1"
  SEVERITY WARNING;
  nq <= not ((((i0 and i1) and i2) or (((i3 or i4) or i5) and i6))) after 1200 ps;
END;
