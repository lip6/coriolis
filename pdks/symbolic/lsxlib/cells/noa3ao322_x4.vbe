ENTITY noa3ao322_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3250;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT cin_i3	 : NATURAL := 9;
  CONSTANT cin_i4	 : NATURAL := 9;
  CONSTANT cin_i5	 : NATURAL := 9;
  CONSTANT cin_i6	 : NATURAL := 9;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rdown_i3_nq	 : NATURAL := 810;
  CONSTANT rdown_i4_nq	 : NATURAL := 810;
  CONSTANT rdown_i5_nq	 : NATURAL := 810;
  CONSTANT rdown_i6_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT rup_i3_nq	 : NATURAL := 890;
  CONSTANT rup_i4_nq	 : NATURAL := 890;
  CONSTANT rup_i5_nq	 : NATURAL := 890;
  CONSTANT rup_i6_nq	 : NATURAL := 890;
  CONSTANT tplh_i6_nq	 : NATURAL := 718;
  CONSTANT tphl_i3_nq	 : NATURAL := 729;
  CONSTANT tphl_i6_nq	 : NATURAL := 738;
  CONSTANT tphl_i0_nq	 : NATURAL := 819;
  CONSTANT tphl_i4_nq	 : NATURAL := 821;
  CONSTANT tplh_i2_nq	 : NATURAL := 874;
  CONSTANT tplh_i5_nq	 : NATURAL := 900;
  CONSTANT tphl_i5_nq	 : NATURAL := 907;
  CONSTANT tphl_i1_nq	 : NATURAL := 914;
  CONSTANT tplh_i4_nq	 : NATURAL := 924;
  CONSTANT tplh_i3_nq	 : NATURAL := 926;
  CONSTANT tplh_i1_nq	 : NATURAL := 931;
  CONSTANT tplh_i0_nq	 : NATURAL := 987;
  CONSTANT tphl_i2_nq	 : NATURAL := 990;
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
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa3ao322_x4;

ARCHITECTURE behaviour_data_flow OF noa3ao322_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa3ao322_x4"
  SEVERITY WARNING;
  nq <= not ((((i0 and i1) and i2) or (((i3 or i4) or i5) and i6))) after 1600 ps;
END;
