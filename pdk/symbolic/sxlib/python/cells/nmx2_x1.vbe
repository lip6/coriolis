ENTITY nmx2_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_cmd	 : NATURAL := 21;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT rdown_cmd_nq	 : NATURAL := 2850;
  CONSTANT rdown_i0_nq	 : NATURAL := 2850;
  CONSTANT rdown_i1_nq	 : NATURAL := 2850;
  CONSTANT rup_cmd_nq	 : NATURAL := 3210;
  CONSTANT rup_i0_nq	 : NATURAL := 3210;
  CONSTANT rup_i1_nq	 : NATURAL := 3210;
  CONSTANT tphl_i0_nq	 : NATURAL := 217;
  CONSTANT tphl_i1_nq	 : NATURAL := 217;
  CONSTANT tphl_cmd_nq	 : NATURAL := 218;
  CONSTANT tplh_i0_nq	 : NATURAL := 256;
  CONSTANT tplh_i1_nq	 : NATURAL := 256;
  CONSTANT tplh_cmd_nq	 : NATURAL := 287;
  CONSTANT tphh_cmd_nq	 : NATURAL := 379;
  CONSTANT tpll_cmd_nq	 : NATURAL := 410;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  cmd	 : in  BIT;
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nmx2_x1;

ARCHITECTURE behaviour_data_flow OF nmx2_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nmx2_x1"
  SEVERITY WARNING;
  nq <= not (((i0 and not (cmd)) or (i1 and cmd))) after 1000 ps;
END;
