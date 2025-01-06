ENTITY nao22_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT cin_i2	 : NATURAL := 15;
  CONSTANT rdown_i0_nq	 : NATURAL := 2850;
  CONSTANT rdown_i1_nq	 : NATURAL := 2850;
  CONSTANT rdown_i2_nq	 : NATURAL := 2850;
  CONSTANT rup_i0_nq	 : NATURAL := 3210;
  CONSTANT rup_i1_nq	 : NATURAL := 3210;
  CONSTANT rup_i2_nq	 : NATURAL := 1790;
  CONSTANT tphl_i2_nq	 : NATURAL := 165;
  CONSTANT tphl_i1_nq	 : NATURAL := 218;
  CONSTANT tplh_i0_nq	 : NATURAL := 226;
  CONSTANT tplh_i2_nq	 : NATURAL := 238;
  CONSTANT tplh_i1_nq	 : NATURAL := 287;
  CONSTANT tphl_i0_nq	 : NATURAL := 294;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nao22_x1;

ARCHITECTURE behaviour_data_flow OF nao22_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nao22_x1"
  SEVERITY WARNING;
  nq <= not (((i0 or i1) and i2)) after 900 ps;
END;
