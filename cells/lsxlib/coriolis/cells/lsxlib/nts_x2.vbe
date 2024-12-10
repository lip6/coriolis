ENTITY nts_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_cmd	 : NATURAL := 18;
  CONSTANT cin_i	 : NATURAL := 28;
  CONSTANT rdown_cmd_nq	 : NATURAL := 1430;
  CONSTANT rdown_i_nq	 : NATURAL := 1430;
  CONSTANT rup_cmd_nq	 : NATURAL := 1600;
  CONSTANT rup_i_nq	 : NATURAL := 1600;
  CONSTANT tphl_cmd_nq	 : NATURAL := 33;
  CONSTANT tphl_i_nq	 : NATURAL := 167;
  CONSTANT tplh_i_nq	 : NATURAL := 201;
  CONSTANT tphh_cmd_nq	 : NATURAL := 330;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  cmd	 : in  BIT;
  i	 : in  BIT;
  nq	 : out MUX_BIT BUS;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nts_x2;

ARCHITECTURE behaviour_data_flow OF nts_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nts_x2"
  SEVERITY WARNING;

  label0 : BLOCK (cmd = '1')
  BEGIN
    nq <= GUARDED not (i) after 900 ps;
  END BLOCK label0;

END;
