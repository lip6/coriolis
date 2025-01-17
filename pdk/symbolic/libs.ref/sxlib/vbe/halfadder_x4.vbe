ENTITY halfadder_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 4500;
  CONSTANT cin_a	 : NATURAL := 27;
  CONSTANT cin_b	 : NATURAL := 22;
  CONSTANT rdown_a_cout	 : NATURAL := 810;
  CONSTANT rdown_a_sout	 : NATURAL := 810;
  CONSTANT rdown_b_cout	 : NATURAL := 810;
  CONSTANT rdown_b_sout	 : NATURAL := 810;
  CONSTANT rup_a_cout	 : NATURAL := 890;
  CONSTANT rup_a_sout	 : NATURAL := 890;
  CONSTANT rup_b_cout	 : NATURAL := 890;
  CONSTANT rup_b_sout	 : NATURAL := 890;
  CONSTANT tphh_a_cout	 : NATURAL := 467;
  CONSTANT tpll_b_cout	 : NATURAL := 480;
  CONSTANT tpll_a_cout	 : NATURAL := 494;
  CONSTANT tphh_b_cout	 : NATURAL := 500;
  CONSTANT tphh_a_sout	 : NATURAL := 527;
  CONSTANT tpll_b_sout	 : NATURAL := 594;
  CONSTANT tphl_b_sout	 : NATURAL := 607;
  CONSTANT tplh_b_sout	 : NATURAL := 642;
  CONSTANT tphh_b_sout	 : NATURAL := 655;
  CONSTANT tphl_a_sout	 : NATURAL := 656;
  CONSTANT tpll_a_sout	 : NATURAL := 665;
  CONSTANT tplh_a_sout	 : NATURAL := 692;
  CONSTANT transistors	 : NATURAL := 24
);
PORT (
  a	 : in  BIT;
  b	 : in  BIT;
  cout	 : out BIT;
  sout	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END halfadder_x4;

ARCHITECTURE behaviour_data_flow OF halfadder_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on halfadder_x4"
  SEVERITY WARNING;
  sout <= (a xor b) after 1300 ps;
  cout <= (a and b) after 1100 ps;
END;
