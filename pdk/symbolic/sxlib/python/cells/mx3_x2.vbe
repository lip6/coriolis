ENTITY mx3_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3250;
  CONSTANT cin_cmd0	 : NATURAL := 15;
  CONSTANT cin_cmd1	 : NATURAL := 15;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT rdown_cmd0_q	 : NATURAL := 1620;
  CONSTANT rdown_cmd1_q	 : NATURAL := 1620;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rup_cmd0_q	 : NATURAL := 1790;
  CONSTANT rup_cmd1_q	 : NATURAL := 1790;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT tphh_i0_q	 : NATURAL := 538;
  CONSTANT tphh_cmd0_q	 : NATURAL := 573;
  CONSTANT tphh_i1_q	 : NATURAL := 654;
  CONSTANT tphh_i2_q	 : NATURAL := 654;
  CONSTANT tpll_i0_q	 : NATURAL := 658;
  CONSTANT tphh_cmd1_q	 : NATURAL := 664;
  CONSTANT tpll_cmd0_q	 : NATURAL := 680;
  CONSTANT tplh_cmd1_q	 : NATURAL := 738;
  CONSTANT tphl_cmd1_q	 : NATURAL := 739;
  CONSTANT tplh_cmd0_q	 : NATURAL := 768;
  CONSTANT tphl_cmd0_q	 : NATURAL := 792;
  CONSTANT tpll_i1_q	 : NATURAL := 808;
  CONSTANT tpll_i2_q	 : NATURAL := 808;
  CONSTANT tpll_cmd1_q	 : NATURAL := 817;
  CONSTANT transistors	 : NATURAL := 20
);
PORT (
  cmd0	 : in  BIT;
  cmd1	 : in  BIT;
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END mx3_x2;

ARCHITECTURE behaviour_data_flow OF mx3_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on mx3_x2"
  SEVERITY WARNING;
  q <= ((not (cmd0) and i0) or (cmd0 and ((cmd1 and i1) or (not (cmd1)
  and i2)))) after 1400 ps;
END;
