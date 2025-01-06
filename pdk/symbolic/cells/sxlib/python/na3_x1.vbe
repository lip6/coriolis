ENTITY na3_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT rdown_i0_nq	 : NATURAL := 4120;
  CONSTANT rdown_i1_nq	 : NATURAL := 4120;
  CONSTANT rdown_i2_nq	 : NATURAL := 4120;
  CONSTANT rup_i0_nq	 : NATURAL := 3720;
  CONSTANT rup_i1_nq	 : NATURAL := 3720;
  CONSTANT rup_i2_nq	 : NATURAL := 3720;
  CONSTANT tphl_i0_nq	 : NATURAL := 119;
  CONSTANT tphl_i1_nq	 : NATURAL := 171;
  CONSTANT tphl_i2_nq	 : NATURAL := 193;
  CONSTANT tplh_i2_nq	 : NATURAL := 265;
  CONSTANT tplh_i1_nq	 : NATURAL := 316;
  CONSTANT tplh_i0_nq	 : NATURAL := 363;
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
END na3_x1;

ARCHITECTURE behaviour_data_flow OF na3_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on na3_x1"
  SEVERITY WARNING;
  nq <= not (((i0 and i1) and i2)) after 1000 ps;
END;
