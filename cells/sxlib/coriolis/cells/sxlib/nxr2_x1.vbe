ENTITY nxr2_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2250;
  CONSTANT cin_i0	 : NATURAL := 21;
  CONSTANT cin_i1	 : NATURAL := 22;
  CONSTANT rdown_i0_nq	 : NATURAL := 2850;
  CONSTANT rdown_i1_nq	 : NATURAL := 2850;
  CONSTANT rup_i0_nq	 : NATURAL := 3210;
  CONSTANT rup_i1_nq	 : NATURAL := 3210;
  CONSTANT tphl_i1_nq	 : NATURAL := 156;
  CONSTANT tphl_i0_nq	 : NATURAL := 288;
  CONSTANT tplh_i0_nq	 : NATURAL := 293;
  CONSTANT tplh_i1_nq	 : NATURAL := 327;
  CONSTANT tphh_i0_nq	 : NATURAL := 366;
  CONSTANT tpll_i0_nq	 : NATURAL := 389;
  CONSTANT tphh_i1_nq	 : NATURAL := 395;
  CONSTANT tpll_i1_nq	 : NATURAL := 503;
  CONSTANT transistors	 : NATURAL := 12
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nxr2_x1;

ARCHITECTURE behaviour_data_flow OF nxr2_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nxr2_x1"
  SEVERITY WARNING;
  nq <= not ((i0 xor i1)) after 1100 ps;
END;
