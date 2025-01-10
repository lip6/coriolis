ENTITY noa2ao222_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT cin_i2	 : NATURAL := 13;
  CONSTANT cin_i3	 : NATURAL := 13;
  CONSTANT cin_i4	 : NATURAL := 13;
  CONSTANT rdown_i0_nq	 : NATURAL := 3210;
  CONSTANT rdown_i1_nq	 : NATURAL := 3210;
  CONSTANT rdown_i2_nq	 : NATURAL := 3210;
  CONSTANT rdown_i3_nq	 : NATURAL := 3210;
  CONSTANT rdown_i4_nq	 : NATURAL := 3210;
  CONSTANT rup_i0_nq	 : NATURAL := 5260;
  CONSTANT rup_i1_nq	 : NATURAL := 5260;
  CONSTANT rup_i2_nq	 : NATURAL := 5260;
  CONSTANT rup_i3_nq	 : NATURAL := 5260;
  CONSTANT rup_i4_nq	 : NATURAL := 3750;
  CONSTANT tphl_i2_nq	 : NATURAL := 186;
  CONSTANT tphl_i4_nq	 : NATURAL := 240;
  CONSTANT tphl_i3_nq	 : NATURAL := 256;
  CONSTANT tplh_i4_nq	 : NATURAL := 309;
  CONSTANT tphl_i0_nq	 : NATURAL := 348;
  CONSTANT tplh_i1_nq	 : NATURAL := 378;
  CONSTANT tplh_i0_nq	 : NATURAL := 422;
  CONSTANT tphl_i1_nq	 : NATURAL := 440;
  CONSTANT tplh_i3_nq	 : NATURAL := 459;
  CONSTANT tplh_i2_nq	 : NATURAL := 473;
  CONSTANT transistors	 : NATURAL := 10
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
END noa2ao222_x1;

ARCHITECTURE behaviour_data_flow OF noa2ao222_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa2ao222_x1"
  SEVERITY WARNING;
  nq <= not (((i0 and i1) or ((i2 or i3) and i4))) after 1100 ps;
END;
