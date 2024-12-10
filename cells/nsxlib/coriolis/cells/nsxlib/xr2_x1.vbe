ENTITY xr2_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2250;
  CONSTANT cin_i0	 : NATURAL := 21;
  CONSTANT cin_i1	 : NATURAL := 22;
  CONSTANT rdown_i0_q	 : NATURAL := 2850;
  CONSTANT rdown_i1_q	 : NATURAL := 2850;
  CONSTANT rup_i0_q	 : NATURAL := 3210;
  CONSTANT rup_i1_q	 : NATURAL := 3210;
  CONSTANT tplh_i1_q	 : NATURAL := 261;
  CONSTANT tphl_i0_q	 : NATURAL := 292;
  CONSTANT tplh_i0_q	 : NATURAL := 293;
  CONSTANT tphh_i0_q	 : NATURAL := 366;
  CONSTANT tphl_i1_q	 : NATURAL := 377;
  CONSTANT tpll_i1_q	 : NATURAL := 388;
  CONSTANT tpll_i0_q	 : NATURAL := 389;
  CONSTANT tphh_i1_q	 : NATURAL := 405;
  CONSTANT transistors	 : NATURAL := 12
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  q	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END xr2_x1;

ARCHITECTURE behaviour_data_flow OF xr2_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on xr2_x1"
  SEVERITY WARNING;
  q <= (i0 xor i1) after 1000 ps;
END;
