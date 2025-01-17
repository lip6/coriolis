ENTITY nts_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_cmd	 : NATURAL := 14;
  CONSTANT cin_i	 : NATURAL := 14;
  CONSTANT rdown_cmd_nq	 : NATURAL := 2850;
  CONSTANT rdown_i_nq	 : NATURAL := 2850;
  CONSTANT rup_cmd_nq	 : NATURAL := 3210;
  CONSTANT rup_i_nq	 : NATURAL := 3210;
  CONSTANT tphl_cmd_nq	 : NATURAL := 41;
  CONSTANT tphl_i_nq	 : NATURAL := 169;
  CONSTANT tplh_i_nq	 : NATURAL := 201;
  CONSTANT tphh_cmd_nq	 : NATURAL := 249;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  cmd	 : in  BIT;
  i	 : in  BIT;
  nq	 : out MUX_BIT BUS;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nts_x1;

ARCHITECTURE behaviour_data_flow OF nts_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nts_x1"
  SEVERITY WARNING;

  label0 : BLOCK (cmd = '1')
  BEGIN
    nq <= GUARDED not (i) after 800 ps;
  END BLOCK label0;

END;
