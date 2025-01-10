ENTITY no2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT tplh_i0_nq	 : NATURAL := 447;
  CONSTANT tplh_i1_nq	 : NATURAL := 504;
  CONSTANT tphl_i1_nq	 : NATURAL := 522;
  CONSTANT tphl_i0_nq	 : NATURAL := 618;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END no2_x4;

ARCHITECTURE behaviour_data_flow OF no2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on no2_x4"
  SEVERITY WARNING;
  nq <= not ((i0 or i1)) after 1200 ps;
END;
