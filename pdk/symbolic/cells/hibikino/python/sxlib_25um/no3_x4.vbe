ENTITY no3_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 12;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT tphl_i2_nq	 : NATURAL := 545;
  CONSTANT tplh_i0_nq	 : NATURAL := 561;
  CONSTANT tplh_i1_nq	 : NATURAL := 623;
  CONSTANT tphl_i1_nq	 : NATURAL := 638;
  CONSTANT tplh_i2_nq	 : NATURAL := 640;
  CONSTANT tphl_i0_nq	 : NATURAL := 722;
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
END no3_x4;

ARCHITECTURE behaviour_data_flow OF no3_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on no3_x4"
  SEVERITY WARNING;
  nq <= not (((i0 or i1) or i2)) after 1300 ps;
END;
