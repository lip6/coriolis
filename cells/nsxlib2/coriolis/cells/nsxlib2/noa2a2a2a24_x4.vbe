ENTITY noa2a2a2a24_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 4250;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT cin_i2	 : NATURAL := 14;
  CONSTANT cin_i3	 : NATURAL := 13;
  CONSTANT cin_i4	 : NATURAL := 13;
  CONSTANT cin_i5	 : NATURAL := 13;
  CONSTANT cin_i6	 : NATURAL := 14;
  CONSTANT cin_i7	 : NATURAL := 14;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rdown_i3_nq	 : NATURAL := 810;
  CONSTANT rdown_i4_nq	 : NATURAL := 810;
  CONSTANT rdown_i5_nq	 : NATURAL := 810;
  CONSTANT rdown_i6_nq	 : NATURAL := 810;
  CONSTANT rdown_i7_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT rup_i3_nq	 : NATURAL := 890;
  CONSTANT rup_i4_nq	 : NATURAL := 890;
  CONSTANT rup_i5_nq	 : NATURAL := 890;
  CONSTANT rup_i6_nq	 : NATURAL := 890;
  CONSTANT rup_i7_nq	 : NATURAL := 890;
  CONSTANT tphl_i7_nq	 : NATURAL := 525;
  CONSTANT tphl_i6_nq	 : NATURAL := 606;
  CONSTANT tphl_i5_nq	 : NATURAL := 649;
  CONSTANT tphl_i4_nq	 : NATURAL := 748;
  CONSTANT tphl_i2_nq	 : NATURAL := 867;
  CONSTANT tphl_i0_nq	 : NATURAL := 966;
  CONSTANT tphl_i3_nq	 : NATURAL := 990;
  CONSTANT tplh_i6_nq	 : NATURAL := 999;
  CONSTANT tplh_i1_nq	 : NATURAL := 1005;
  CONSTANT tplh_i0_nq	 : NATURAL := 1049;
  CONSTANT tplh_i7_nq	 : NATURAL := 1052;
  CONSTANT tplh_i3_nq	 : NATURAL := 1061;
  CONSTANT tplh_i4_nq	 : NATURAL := 1061;
  CONSTANT tphl_i1_nq	 : NATURAL := 1097;
  CONSTANT tplh_i2_nq	 : NATURAL := 1106;
  CONSTANT tplh_i5_nq	 : NATURAL := 1109;
  CONSTANT transistors	 : NATURAL := 22
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
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa2a2a2a24_x4;

ARCHITECTURE behaviour_data_flow OF noa2a2a2a24_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa2a2a2a24_x4"
  SEVERITY WARNING;
  nq <= not (((((i0 and i1) or (i2 and i3)) or (i4 and i5)) or (i6 and
  i7))) after 1700 ps;
END;
