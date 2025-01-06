ENTITY noa2ao222_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3000;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT cin_i3	 : NATURAL := 11;
  CONSTANT cin_i4	 : NATURAL := 11;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rdown_i3_nq	 : NATURAL := 810;
  CONSTANT rdown_i4_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT rup_i3_nq	 : NATURAL := 890;
  CONSTANT rup_i4_nq	 : NATURAL := 890;
  CONSTANT tphl_i2_nq	 : NATURAL := 638;
  CONSTANT tplh_i4_nq	 : NATURAL := 664;
  CONSTANT tphl_i0_nq	 : NATURAL := 684;
  CONSTANT tphl_i4_nq	 : NATURAL := 718;
  CONSTANT tphl_i3_nq	 : NATURAL := 732;
  CONSTANT tplh_i1_nq	 : NATURAL := 758;
  CONSTANT tphl_i1_nq	 : NATURAL := 780;
  CONSTANT tplh_i3_nq	 : NATURAL := 795;
  CONSTANT tplh_i0_nq	 : NATURAL := 801;
  CONSTANT tplh_i2_nq	 : NATURAL := 809;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa2ao222_x4;

ARCHITECTURE behaviour_data_flow OF noa2ao222_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa2ao222_x4"
  SEVERITY WARNING;
  nq <= not (((i0 and i1) or ((i2 or i3) and i4))) after 1400 ps;
END;
