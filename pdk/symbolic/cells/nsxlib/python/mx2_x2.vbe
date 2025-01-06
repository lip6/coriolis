ENTITY mx2_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2250;
  CONSTANT cin_cmd	 : NATURAL := 17;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT rdown_cmd_q	 : NATURAL := 1620;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rup_cmd_q	 : NATURAL := 1790;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT tphh_i0_q	 : NATURAL := 451;
  CONSTANT tphh_i1_q	 : NATURAL := 451;
  CONSTANT tpll_i0_q	 : NATURAL := 469;
  CONSTANT tpll_i1_q	 : NATURAL := 469;
  CONSTANT tphh_cmd_q	 : NATURAL := 484;
  CONSTANT tphl_cmd_q	 : NATURAL := 485;
  CONSTANT tpll_cmd_q	 : NATURAL := 522;
  CONSTANT tplh_cmd_q	 : NATURAL := 534;
  CONSTANT transistors	 : NATURAL := 12
);
PORT (
  cmd	 : in  BIT;
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END mx2_x2;

ARCHITECTURE behaviour_data_flow OF mx2_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on mx2_x2"
  SEVERITY WARNING;
  q <= ((i1 and cmd) or (not (cmd) and i0)) after 1100 ps;
END;
