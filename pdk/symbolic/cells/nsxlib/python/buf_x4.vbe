ENTITY buf_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1250;
  CONSTANT cin_i	 : NATURAL := 9;
  CONSTANT rdown_i_q	 : NATURAL := 810;
  CONSTANT rup_i_q	 : NATURAL := 890;
  CONSTANT tphh_i_q	 : NATURAL := 379;
  CONSTANT tpll_i_q	 : NATURAL := 409;
  CONSTANT transistors	 : NATURAL := 6
);
PORT (
  i	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END buf_x4;

ARCHITECTURE behaviour_data_flow OF buf_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on buf_x4"
  SEVERITY WARNING;
  q <= i after 1000 ps;
END;
