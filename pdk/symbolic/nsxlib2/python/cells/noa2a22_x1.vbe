ENTITY noa2a22_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT cin_i2	 : NATURAL := 14;
  CONSTANT cin_i3	 : NATURAL := 14;
  CONSTANT rdown_i0_nq	 : NATURAL := 2850;
  CONSTANT rdown_i1_nq	 : NATURAL := 2850;
  CONSTANT rdown_i2_nq	 : NATURAL := 2850;
  CONSTANT rdown_i3_nq	 : NATURAL := 2850;
  CONSTANT rup_i0_nq	 : NATURAL := 3210;
  CONSTANT rup_i1_nq	 : NATURAL := 3210;
  CONSTANT rup_i2_nq	 : NATURAL := 3210;
  CONSTANT rup_i3_nq	 : NATURAL := 3210;
  CONSTANT tphl_i0_nq	 : NATURAL := 151;
  CONSTANT tphl_i1_nq	 : NATURAL := 218;
  CONSTANT tplh_i3_nq	 : NATURAL := 256;
  CONSTANT tphl_i2_nq	 : NATURAL := 284;
  CONSTANT tplh_i1_nq	 : NATURAL := 287;
  CONSTANT tplh_i2_nq	 : NATURAL := 289;
  CONSTANT tplh_i0_nq	 : NATURAL := 327;
  CONSTANT tphl_i3_nq	 : NATURAL := 372;
  CONSTANT transistors	 : NATURAL := 8
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa2a22_x1;

ARCHITECTURE behaviour_data_flow OF noa2a22_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa2a22_x1"
  SEVERITY WARNING;
  nq <= not (((i0 and i1) or (i2 and i3))) after 1000 ps;
END;
