ENTITY mx2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2500;
  CONSTANT cin_cmd	 : NATURAL := 17;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 9;
  CONSTANT rdown_cmd_q	 : NATURAL := 810;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rup_cmd_q	 : NATURAL := 890;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT tphh_i0_q	 : NATURAL := 564;
  CONSTANT tphh_i1_q	 : NATURAL := 564;
  CONSTANT tphl_cmd_q	 : NATURAL := 574;
  CONSTANT tpll_i0_q	 : NATURAL := 576;
  CONSTANT tpll_i1_q	 : NATURAL := 576;
  CONSTANT tphh_cmd_q	 : NATURAL := 615;
  CONSTANT tplh_cmd_q	 : NATURAL := 631;
  CONSTANT tpll_cmd_q	 : NATURAL := 647;
  CONSTANT transistors	 : NATURAL := 14
);
PORT (
  cmd	 : in  BIT;
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END mx2_x4;

ARCHITECTURE behaviour_data_flow OF mx2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on mx2_x4"
  SEVERITY WARNING;
  q <= ((i1 and cmd) or (not (cmd) and i0)) after 1200 ps;
END;
