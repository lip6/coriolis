ENTITY nmx3_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3000;
  CONSTANT cin_cmd0	 : NATURAL := 15;
  CONSTANT cin_cmd1	 : NATURAL := 15;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT rdown_cmd0_nq	 : NATURAL := 7420;
  CONSTANT rdown_cmd1_nq	 : NATURAL := 7420;
  CONSTANT rdown_i0_nq	 : NATURAL := 5140;
  CONSTANT rdown_i1_nq	 : NATURAL := 7420;
  CONSTANT rdown_i2_nq	 : NATURAL := 7420;
  CONSTANT rup_cmd0_nq	 : NATURAL := 9760;
  CONSTANT rup_cmd1_nq	 : NATURAL := 9760;
  CONSTANT rup_i0_nq	 : NATURAL := 6680;
  CONSTANT rup_i1_nq	 : NATURAL := 9760;
  CONSTANT rup_i2_nq	 : NATURAL := 9760;
  CONSTANT tphl_i0_nq	 : NATURAL := 315;
  CONSTANT tphl_cmd0_nq	 : NATURAL := 356;
  CONSTANT tphl_cmd1_nq	 : NATURAL := 414;
  CONSTANT tphl_i1_nq	 : NATURAL := 429;
  CONSTANT tphl_i2_nq	 : NATURAL := 429;
  CONSTANT tplh_i0_nq	 : NATURAL := 441;
  CONSTANT tplh_cmd0_nq	 : NATURAL := 495;
  CONSTANT tphh_cmd1_nq	 : NATURAL := 519;
  CONSTANT tpll_cmd1_nq	 : NATURAL := 520;
  CONSTANT tplh_cmd1_nq	 : NATURAL := 566;
  CONSTANT tphh_cmd0_nq	 : NATURAL := 582;
  CONSTANT tplh_i1_nq	 : NATURAL := 582;
  CONSTANT tplh_i2_nq	 : NATURAL := 582;
  CONSTANT tpll_cmd0_nq	 : NATURAL := 586;
  CONSTANT transistors	 : NATURAL := 18
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
END nmx3_x1;

ARCHITECTURE behaviour_data_flow OF nmx3_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nmx3_x1"
  SEVERITY WARNING;
  nq <= not (((not (cmd0) and i0) or (cmd0 and ((cmd1 and i1) or (not
  (cmd1) and i2))))) after 1200 ps;
END;
