ENTITY nxr2_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3000;
  CONSTANT cin_i0	 : NATURAL := 20;
  CONSTANT cin_i1	 : NATURAL := 21;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT tpll_i1_nq	 : NATURAL := 453;
  CONSTANT tphh_i0_nq	 : NATURAL := 469;
  CONSTANT tpll_i0_nq	 : NATURAL := 481;
  CONSTANT tphl_i0_nq	 : NATURAL := 522;
  CONSTANT tplh_i1_nq	 : NATURAL := 542;
  CONSTANT tphl_i1_nq	 : NATURAL := 553;
  CONSTANT tplh_i0_nq	 : NATURAL := 553;
  CONSTANT tphh_i1_nq	 : NATURAL := 568;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END nxr2_x4;

ARCHITECTURE behaviour_data_flow OF nxr2_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nxr2_x4"
  SEVERITY WARNING;
  nq <= not ((i0 xor i1)) after 1200 ps;
END;
