ENTITY na2_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1000;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT rdown_i0_nq	 : NATURAL := 2850;
  CONSTANT rdown_i1_nq	 : NATURAL := 2850;
  CONSTANT rup_i0_nq	 : NATURAL := 3720;
  CONSTANT rup_i1_nq	 : NATURAL := 3720;
  CONSTANT tphl_i0_nq	 : NATURAL := 59;
  CONSTANT tphl_i1_nq	 : NATURAL := 111;
  CONSTANT tplh_i1_nq	 : NATURAL := 234;
  CONSTANT tplh_i0_nq	 : NATURAL := 288;
  CONSTANT transistors	 : NATURAL := 4
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END na2_x1;

ARCHITECTURE behaviour_data_flow OF na2_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on na2_x1"
  SEVERITY WARNING;
  nq <= not ((i0 and i1)) after 900 ps;
END;
