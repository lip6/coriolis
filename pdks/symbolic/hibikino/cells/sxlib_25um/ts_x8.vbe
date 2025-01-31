ENTITY ts_x8 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3250;
  CONSTANT cin_cmd	 : NATURAL := 19;
  CONSTANT cin_i	 : NATURAL := 8;
  CONSTANT rdown_cmd_q	 : NATURAL := 400;
  CONSTANT rdown_i_q	 : NATURAL := 400;
  CONSTANT rup_cmd_q	 : NATURAL := 450;
  CONSTANT rup_i_q	 : NATURAL := 450;
  CONSTANT tphl_cmd_q	 : NATURAL := 466;
  CONSTANT tpll_i_q	 : NATURAL := 569;
  CONSTANT tphh_i_q	 : NATURAL := 613;
  CONSTANT tphh_cmd_q	 : NATURAL := 626;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  cmd	 : in  BIT;
  i	 : in  BIT;
  q	 : out MUX_BIT BUS;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END ts_x8;

ARCHITECTURE behaviour_data_flow OF ts_x8 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on ts_x8"
  SEVERITY WARNING;

  label0 : BLOCK (cmd = '1')
  BEGIN
    q <= GUARDED i after 1200 ps;
  END BLOCK label0;

END;
