ENTITY halfadder_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 4000;
  CONSTANT cin_a	 : NATURAL := 27;
  CONSTANT cin_b	 : NATURAL := 22;
  CONSTANT rdown_a_cout	 : NATURAL := 1620;
  CONSTANT rdown_a_sout	 : NATURAL := 1620;
  CONSTANT rdown_b_cout	 : NATURAL := 1620;
  CONSTANT rdown_b_sout	 : NATURAL := 1620;
  CONSTANT rup_a_cout	 : NATURAL := 1790;
  CONSTANT rup_a_sout	 : NATURAL := 1790;
  CONSTANT rup_b_cout	 : NATURAL := 1790;
  CONSTANT rup_b_sout	 : NATURAL := 1790;
  CONSTANT tphh_a_cout	 : NATURAL := 361;
  CONSTANT tpll_b_cout	 : NATURAL := 383;
  CONSTANT tphh_b_cout	 : NATURAL := 386;
  CONSTANT tpll_a_cout	 : NATURAL := 398;
  CONSTANT tphh_a_sout	 : NATURAL := 421;
  CONSTANT tpll_b_sout	 : NATURAL := 497;
  CONSTANT tphl_b_sout	 : NATURAL := 531;
  CONSTANT tplh_b_sout	 : NATURAL := 556;
  CONSTANT tphh_b_sout	 : NATURAL := 558;
  CONSTANT tpll_a_sout	 : NATURAL := 562;
  CONSTANT tphl_a_sout	 : NATURAL := 575;
  CONSTANT tplh_a_sout	 : NATURAL := 607;
  CONSTANT transistors	 : NATURAL := 20
);
PORT (
  a	 : in  BIT;
  b	 : in  BIT;
  cout	 : out BIT;
  sout	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END halfadder_x2;

ARCHITECTURE behaviour_data_flow OF halfadder_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on halfadder_x2"
  SEVERITY WARNING;
  sout <= (a xor b) after 1200 ps;
  cout <= (a and b) after 1000 ps;
END;
