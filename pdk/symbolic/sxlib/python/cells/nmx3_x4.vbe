ENTITY nmx3_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3750;
  CONSTANT cin_cmd0	 : NATURAL := 15;
  CONSTANT cin_cmd1	 : NATURAL := 15;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT rdown_cmd0_nq	 : NATURAL := 810;
  CONSTANT rdown_cmd1_nq	 : NATURAL := 810;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rup_cmd0_nq	 : NATURAL := 890;
  CONSTANT rup_cmd1_nq	 : NATURAL := 890;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT tphl_i0_nq	 : NATURAL := 748;
  CONSTANT tphl_cmd0_nq	 : NATURAL := 790;
  CONSTANT tphl_cmd1_nq	 : NATURAL := 866;
  CONSTANT tphl_i1_nq	 : NATURAL := 869;
  CONSTANT tphl_i2_nq	 : NATURAL := 869;
  CONSTANT tplh_i0_nq	 : NATURAL := 900;
  CONSTANT tplh_cmd0_nq	 : NATURAL := 936;
  CONSTANT tpll_cmd1_nq	 : NATURAL := 952;
  CONSTANT tphh_cmd1_nq	 : NATURAL := 981;
  CONSTANT tpll_cmd0_nq	 : NATURAL := 993;
  CONSTANT tphh_cmd0_nq	 : NATURAL := 1041;
  CONSTANT tplh_cmd1_nq	 : NATURAL := 1048;
  CONSTANT tplh_i1_nq	 : NATURAL := 1053;
  CONSTANT tplh_i2_nq	 : NATURAL := 1053;
  CONSTANT transistors	 : NATURAL := 24
);
PORT (
  cmd0	 : in  BIT;
  cmd1	 : in  BIT;
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nmx3_x4;

ARCHITECTURE behaviour_data_flow OF nmx3_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nmx3_x4"
  SEVERITY WARNING;
  nq <= not (((not (cmd0) and i0) or (cmd0 and ((cmd1 and i1) or (not
  (cmd1) and i2))))) after 1700 ps;
END;
