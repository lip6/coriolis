ENTITY ts_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2500;
  CONSTANT cin_cmd	 : NATURAL := 19;
  CONSTANT cin_i	 : NATURAL := 8;
  CONSTANT rdown_cmd_q	 : NATURAL := 810;
  CONSTANT rdown_i_q	 : NATURAL := 810;
  CONSTANT rup_cmd_q	 : NATURAL := 890;
  CONSTANT rup_i_q	 : NATURAL := 890;
  CONSTANT tphl_cmd_q	 : NATURAL := 409;
  CONSTANT tpll_i_q	 : NATURAL := 444;
  CONSTANT tphh_i_q	 : NATURAL := 475;
  CONSTANT tphh_cmd_q	 : NATURAL := 492;
  CONSTANT transistors	 : NATURAL := 12
);
PORT (
  cmd	 : in  BIT;
  i	 : in  BIT;
  q	 : out MUX_BIT BUS;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END ts_x4;

ARCHITECTURE behaviour_data_flow OF ts_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on ts_x4"
  SEVERITY WARNING;

  label0 : BLOCK (cmd = '1')
  BEGIN
    q <= GUARDED i after 1100 ps;
  END BLOCK label0;

END;
