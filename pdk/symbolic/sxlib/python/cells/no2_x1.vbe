ENTITY no2_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1000;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 12;
  CONSTANT rdown_i0_nq	 : NATURAL := 3640;
  CONSTANT rdown_i1_nq	 : NATURAL := 3640;
  CONSTANT rup_i0_nq	 : NATURAL := 3210;
  CONSTANT rup_i1_nq	 : NATURAL := 3210;
  CONSTANT tplh_i0_nq	 : NATURAL := 121;
  CONSTANT tplh_i1_nq	 : NATURAL := 161;
  CONSTANT tphl_i1_nq	 : NATURAL := 193;
  CONSTANT tphl_i0_nq	 : NATURAL := 298;
  CONSTANT transistors	 : NATURAL := 4
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END no2_x1;

ARCHITECTURE behaviour_data_flow OF no2_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on no2_x1"
  SEVERITY WARNING;
  nq <= not ((i0 or i1)) after 900 ps;
END;
