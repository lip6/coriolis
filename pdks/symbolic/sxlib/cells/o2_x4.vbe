ENTITY o2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 10;
  CONSTANT cin_i1	 : NATURAL := 10;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT tpll_i0_q	 : NATURAL := 394;
  CONSTANT tphh_i1_q	 : NATURAL := 427;
  CONSTANT tpll_i1_q	 : NATURAL := 464;
  CONSTANT tphh_i0_q	 : NATURAL := 491;
  CONSTANT transistors	 : NATURAL := 8
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END o2_x4;

ARCHITECTURE behaviour_data_flow OF o2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on o2_x4"
  SEVERITY WARNING;
  q <= (i0 or i1) after 1100 ps;
END;
