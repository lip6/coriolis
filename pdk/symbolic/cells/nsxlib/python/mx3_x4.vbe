ENTITY mx3_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3500;
  CONSTANT cin_cmd0	 : NATURAL := 15;
  CONSTANT cin_cmd1	 : NATURAL := 15;
  CONSTANT cin_i0	 : NATURAL := 9;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT rdown_cmd0_q	 : NATURAL := 810;
  CONSTANT rdown_cmd1_q	 : NATURAL := 810;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rup_cmd0_q	 : NATURAL := 890;
  CONSTANT rup_cmd1_q	 : NATURAL := 890;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT tphh_i0_q	 : NATURAL := 640;
  CONSTANT tphh_cmd0_q	 : NATURAL := 683;
  CONSTANT tphh_i1_q	 : NATURAL := 770;
  CONSTANT tphh_i2_q	 : NATURAL := 770;
  CONSTANT tpll_i0_q	 : NATURAL := 774;
  CONSTANT tpll_cmd0_q	 : NATURAL := 779;
  CONSTANT tphh_cmd1_q	 : NATURAL := 792;
  CONSTANT tplh_cmd0_q	 : NATURAL := 844;
  CONSTANT tplh_cmd1_q	 : NATURAL := 846;
  CONSTANT tphl_cmd1_q	 : NATURAL := 872;
  CONSTANT tphl_cmd0_q	 : NATURAL := 922;
  CONSTANT tpll_i1_q	 : NATURAL := 948;
  CONSTANT tpll_i2_q	 : NATURAL := 948;
  CONSTANT tpll_cmd1_q	 : NATURAL := 967;
  CONSTANT transistors	 : NATURAL := 22
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
END mx3_x4;

ARCHITECTURE behaviour_data_flow OF mx3_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on mx3_x4"
  SEVERITY WARNING;
  q <= ((not (cmd0) and i0) or (cmd0 and ((cmd1 and i1) or (not (cmd1)
  and i2)))) after 1600 ps;
END;
