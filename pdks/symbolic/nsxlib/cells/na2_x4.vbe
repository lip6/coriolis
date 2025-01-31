ENTITY na2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 10;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT tphl_i1_nq	 : NATURAL := 353;
  CONSTANT tphl_i0_nq	 : NATURAL := 412;
  CONSTANT tplh_i0_nq	 : NATURAL := 552;
  CONSTANT tplh_i1_nq	 : NATURAL := 601;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END na2_x4;

ARCHITECTURE behaviour_data_flow OF na2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on na2_x4"
  SEVERITY WARNING;
  nq <= not ((i0 and i1)) after 1200 ps;
END;
