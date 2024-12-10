ENTITY no3_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 12;
  CONSTANT cin_i2	 : NATURAL := 12;
  CONSTANT rdown_i0_nq	 : NATURAL := 3640;
  CONSTANT rdown_i1_nq	 : NATURAL := 3640;
  CONSTANT rdown_i2_nq	 : NATURAL := 3640;
  CONSTANT rup_i0_nq	 : NATURAL := 4690;
  CONSTANT rup_i1_nq	 : NATURAL := 4690;
  CONSTANT rup_i2_nq	 : NATURAL := 4690;
  CONSTANT tplh_i2_nq	 : NATURAL := 192;
  CONSTANT tphl_i1_nq	 : NATURAL := 215;
  CONSTANT tplh_i1_nq	 : NATURAL := 243;
  CONSTANT tplh_i0_nq	 : NATURAL := 246;
  CONSTANT tphl_i0_nq	 : NATURAL := 318;
  CONSTANT tphl_i2_nq	 : NATURAL := 407;
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
END no3_x1;

ARCHITECTURE behaviour_data_flow OF no3_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on no3_x1"
  SEVERITY WARNING;
  nq <= not (((i0 or i1) or i2)) after 1000 ps;
END;
