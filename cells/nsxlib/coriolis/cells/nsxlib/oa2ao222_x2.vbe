ENTITY oa2ao222_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2500;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT cin_i3	 : NATURAL := 11;
  CONSTANT cin_i4	 : NATURAL := 11;
  CONSTANT rdown_i0_q	 : NATURAL := 1620;
  CONSTANT rdown_i1_q	 : NATURAL := 1620;
  CONSTANT rdown_i2_q	 : NATURAL := 1620;
  CONSTANT rdown_i3_q	 : NATURAL := 1620;
  CONSTANT rdown_i4_q	 : NATURAL := 1620;
  CONSTANT rup_i0_q	 : NATURAL := 1790;
  CONSTANT rup_i1_q	 : NATURAL := 1790;
  CONSTANT rup_i2_q	 : NATURAL := 1790;
  CONSTANT rup_i3_q	 : NATURAL := 1790;
  CONSTANT rup_i4_q	 : NATURAL := 1790;
  CONSTANT tpll_i4_q	 : NATURAL := 453;
  CONSTANT tphh_i2_q	 : NATURAL := 464;
  CONSTANT tphh_i0_q	 : NATURAL := 495;
  CONSTANT tpll_i1_q	 : NATURAL := 539;
  CONSTANT tphh_i3_q	 : NATURAL := 556;
  CONSTANT tphh_i4_q	 : NATURAL := 558;
  CONSTANT tpll_i3_q	 : NATURAL := 578;
  CONSTANT tpll_i0_q	 : NATURAL := 581;
  CONSTANT tphh_i1_q	 : NATURAL := 598;
  CONSTANT tpll_i2_q	 : NATURAL := 604;
  CONSTANT transistors	 : NATURAL := 12
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END oa2ao222_x2;

ARCHITECTURE behaviour_data_flow OF oa2ao222_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa2ao222_x2"
  SEVERITY WARNING;
  q <= ((i0 and i1) or (i4 and (i2 or i3))) after 1200 ps;
END;
