ENTITY oa2ao222_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2750;
  CONSTANT cin_i0	 : NATURAL := 11;
  CONSTANT cin_i1	 : NATURAL := 11;
  CONSTANT cin_i2	 : NATURAL := 11;
  CONSTANT cin_i3	 : NATURAL := 11;
  CONSTANT cin_i4	 : NATURAL := 11;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rdown_i2_q	 : NATURAL := 810;
  CONSTANT rdown_i3_q	 : NATURAL := 810;
  CONSTANT rdown_i4_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT rup_i2_q	 : NATURAL := 890;
  CONSTANT rup_i3_q	 : NATURAL := 890;
  CONSTANT rup_i4_q	 : NATURAL := 890;
  CONSTANT tpll_i4_q	 : NATURAL := 529;
  CONSTANT tphh_i2_q	 : NATURAL := 552;
  CONSTANT tphh_i0_q	 : NATURAL := 553;
  CONSTANT tpll_i1_q	 : NATURAL := 616;
  CONSTANT tphh_i3_q	 : NATURAL := 640;
  CONSTANT tphh_i4_q	 : NATURAL := 656;
  CONSTANT tpll_i0_q	 : NATURAL := 657;
  CONSTANT tpll_i3_q	 : NATURAL := 660;
  CONSTANT tphh_i1_q	 : NATURAL := 662;
  CONSTANT tpll_i2_q	 : NATURAL := 693;
  CONSTANT transistors	 : NATURAL := 14
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
END oa2ao222_x4;

ARCHITECTURE behaviour_data_flow OF oa2ao222_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on oa2ao222_x4"
  SEVERITY WARNING;
  q <= ((i0 and i1) or (i4 and (i2 or i3))) after 1300 ps;
END;
