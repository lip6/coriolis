ENTITY buf_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1000;
  CONSTANT cin_i	 : NATURAL := 6;
  CONSTANT rdown_i_q	 : NATURAL := 1620;
  CONSTANT rup_i_q	 : NATURAL := 1790;
  CONSTANT tpll_i_q	 : NATURAL := 391;
  CONSTANT tphh_i_q	 : NATURAL := 409;
  CONSTANT transistors	 : NATURAL := 4
);
PORT (
  i	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END buf_x2;

ARCHITECTURE behaviour_data_flow OF buf_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on buf_x2"
  SEVERITY WARNING;
  q <= i after 1000 ps;
END;
