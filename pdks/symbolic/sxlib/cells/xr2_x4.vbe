ENTITY xr2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3000;
  CONSTANT cin_i0	 : NATURAL := 20;
  CONSTANT cin_i1	 : NATURAL := 21;
  CONSTANT rdown_i0_q	 : NATURAL := 810;
  CONSTANT rdown_i1_q	 : NATURAL := 810;
  CONSTANT rup_i0_q	 : NATURAL := 890;
  CONSTANT rup_i1_q	 : NATURAL := 890;
  CONSTANT tphh_i1_q	 : NATURAL := 357;
  CONSTANT tphh_i0_q	 : NATURAL := 476;
  CONSTANT tpll_i0_q	 : NATURAL := 480;
  CONSTANT tphl_i0_q	 : NATURAL := 521;
  CONSTANT tpll_i1_q	 : NATURAL := 539;
  CONSTANT tphl_i1_q	 : NATURAL := 541;
  CONSTANT tplh_i0_q	 : NATURAL := 560;
  CONSTANT tplh_i1_q	 : NATURAL := 657;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END xr2_x4;

ARCHITECTURE behaviour_data_flow OF xr2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on xr2_x4"
  SEVERITY WARNING;
  q <= (i0 xor i1) after 1300 ps;
END;
