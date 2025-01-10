ENTITY nmx2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3000;
  CONSTANT cin_cmd	 : NATURAL := 17;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT rdown_cmd_nq	 : NATURAL := 810;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rup_cmd_nq	 : NATURAL := 890;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT tphl_i0_nq	 : NATURAL := 610;
  CONSTANT tphl_i1_nq	 : NATURAL := 610;
  CONSTANT tphl_cmd_nq	 : NATURAL := 632;
  CONSTANT tplh_i0_nq	 : NATURAL := 653;
  CONSTANT tplh_i1_nq	 : NATURAL := 653;
  CONSTANT tphh_cmd_nq	 : NATURAL := 688;
  CONSTANT tpll_cmd_nq	 : NATURAL := 703;
  CONSTANT tplh_cmd_nq	 : NATURAL := 708;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  cmd	 : in  BIT;
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nmx2_x4;

ARCHITECTURE behaviour_data_flow OF nmx2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nmx2_x4"
  SEVERITY WARNING;
  nq <= not (((i0 and not (cmd)) or (i1 and cmd))) after 1300 ps;
END;
