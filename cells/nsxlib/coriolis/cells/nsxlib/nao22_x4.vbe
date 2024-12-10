ENTITY nao22_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2500;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT tphl_i2_nq	 : NATURAL := 596;
  CONSTANT tplh_i2_nq	 : NATURAL := 636;
  CONSTANT tplh_i0_nq	 : NATURAL := 650;
  CONSTANT tphl_i1_nq	 : NATURAL := 664;
  CONSTANT tplh_i1_nq	 : NATURAL := 723;
  CONSTANT tphl_i0_nq	 : NATURAL := 732;
  CONSTANT transistors	 : NATURAL := 12
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nao22_x4;

ARCHITECTURE behaviour_data_flow OF nao22_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nao22_x4"
  SEVERITY WARNING;
  nq <= not (((i0 or i1) and i2)) after 1300 ps;
END;
