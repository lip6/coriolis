ENTITY buf_x8 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2000;
  CONSTANT cin_i	 : NATURAL := 15;
  CONSTANT rdown_i_q	 : NATURAL := 400;
  CONSTANT rup_i_q	 : NATURAL := 450;
  CONSTANT tphh_i_q	 : NATURAL := 343;
  CONSTANT tpll_i_q	 : NATURAL := 396;
  CONSTANT transistors	 : NATURAL := 10
);
PORT (
  i	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END buf_x8;

ARCHITECTURE behaviour_data_flow OF buf_x8 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on buf_x8"
  SEVERITY WARNING;
  q <= i after 1000 ps;
END;
