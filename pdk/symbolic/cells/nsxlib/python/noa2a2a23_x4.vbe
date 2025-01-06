ENTITY noa2a2a23_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3250;
  CONSTANT cin_i0	 : NATURAL := 13;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT cin_i2	 : NATURAL := 14;
  CONSTANT cin_i3	 : NATURAL := 14;
  CONSTANT cin_i4	 : NATURAL := 14;
  CONSTANT cin_i5	 : NATURAL := 14;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rdown_i3_nq	 : NATURAL := 810;
  CONSTANT rdown_i4_nq	 : NATURAL := 810;
  CONSTANT rdown_i5_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT rup_i3_nq	 : NATURAL := 890;
  CONSTANT rup_i4_nq	 : NATURAL := 890;
  CONSTANT rup_i5_nq	 : NATURAL := 890;
  CONSTANT tphl_i5_nq	 : NATURAL := 496;
  CONSTANT tphl_i4_nq	 : NATURAL := 574;
  CONSTANT tphl_i2_nq	 : NATURAL := 620;
  CONSTANT tphl_i3_nq	 : NATURAL := 716;
  CONSTANT tplh_i1_nq	 : NATURAL := 778;
  CONSTANT tplh_i0_nq	 : NATURAL := 814;
  CONSTANT tplh_i4_nq	 : NATURAL := 819;
  CONSTANT tplh_i3_nq	 : NATURAL := 833;
  CONSTANT tphl_i0_nq	 : NATURAL := 834;
  CONSTANT tplh_i5_nq	 : NATURAL := 865;
  CONSTANT tplh_i2_nq	 : NATURAL := 873;
  CONSTANT tphl_i1_nq	 : NATURAL := 955;
  CONSTANT transistors	 : NATURAL := 18
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  i5	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa2a2a23_x4;

ARCHITECTURE behaviour_data_flow OF noa2a2a23_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa2a2a23_x4"
  SEVERITY WARNING;
  nq <= not ((((i0 and i1) or (i2 and i3)) or (i4 and i5))) after 1600 ps;
END;
