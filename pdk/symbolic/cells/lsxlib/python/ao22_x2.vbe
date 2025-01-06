ENTITY ao22_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 9;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT tphh_i2_q	 : NATURAL := 420;
  CONSTANT tpll_i2_q	 : NATURAL := 425;
  CONSTANT tpll_i0_q	 : NATURAL := 447;
  CONSTANT tphh_i1_q	 : NATURAL := 493;
  CONSTANT tpll_i1_q	 : NATURAL := 526;
  CONSTANT tphh_i0_q	 : NATURAL := 558;
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
END ao22_x2;

ARCHITECTURE behaviour_data_flow OF ao22_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on ao22_x2"
  SEVERITY WARNING;
  q <= ((i0 or i1) and i2) after 1200 ps;
END;
