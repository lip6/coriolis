ENTITY a3_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 10;
  CONSTANT cin_i2	 : NATURAL := 10;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT tphh_i2_q	 : NATURAL := 290;
  CONSTANT tphh_i1_q	 : NATURAL := 353;
  CONSTANT tphh_i0_q	 : NATURAL := 395;
  CONSTANT tpll_i0_q	 : NATURAL := 435;
  CONSTANT tpll_i1_q	 : NATURAL := 479;
  CONSTANT tpll_i2_q	 : NATURAL := 521;
  CONSTANT transistors	 : NATURAL := 8
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END a3_x2;

ARCHITECTURE behaviour_data_flow OF a3_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on a3_x2"
  SEVERITY WARNING;
  q <= ((i0 and i1) and i2) after 1100 ps;
END;
