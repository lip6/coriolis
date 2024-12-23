ENTITY fulladder_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 5250;
  CONSTANT cin_a1	 : NATURAL := 8;
  CONSTANT cin_a2	 : NATURAL := 8;
  CONSTANT cin_a3	 : NATURAL := 6;
  CONSTANT cin_a4	 : NATURAL := 6;
  CONSTANT cin_b1	 : NATURAL := 8;
  CONSTANT cin_b2	 : NATURAL := 8;
  CONSTANT cin_b3	 : NATURAL := 6;
  CONSTANT cin_b4	 : NATURAL := 6;
  CONSTANT cin_cin1	 : NATURAL := 7;
  CONSTANT cin_cin2	 : NATURAL := 6;
  CONSTANT cin_cin3	 : NATURAL := 6;
  CONSTANT rdown_a1_cout	 : NATURAL := 810;
  CONSTANT rdown_a1_sout	 : NATURAL := 810;
  CONSTANT rdown_a2_cout	 : NATURAL := 810;
  CONSTANT rdown_a2_sout	 : NATURAL := 810;
  CONSTANT rdown_a3_sout	 : NATURAL := 810;
  CONSTANT rdown_a4_sout	 : NATURAL := 810;
  CONSTANT rdown_b1_cout	 : NATURAL := 810;
  CONSTANT rdown_b1_sout	 : NATURAL := 810;
  CONSTANT rdown_b2_cout	 : NATURAL := 810;
  CONSTANT rdown_b2_sout	 : NATURAL := 810;
  CONSTANT rdown_b3_sout	 : NATURAL := 810;
  CONSTANT rdown_b4_sout	 : NATURAL := 810;
  CONSTANT rdown_cin1_cout	 : NATURAL := 810;
  CONSTANT rdown_cin1_sout	 : NATURAL := 810;
  CONSTANT rdown_cin2_sout	 : NATURAL := 810;
  CONSTANT rdown_cin3_sout	 : NATURAL := 810;
  CONSTANT rup_a1_cout	 : NATURAL := 890;
  CONSTANT rup_a1_sout	 : NATURAL := 890;
  CONSTANT rup_a2_cout	 : NATURAL := 890;
  CONSTANT rup_a2_sout	 : NATURAL := 890;
  CONSTANT rup_a3_sout	 : NATURAL := 890;
  CONSTANT rup_a4_sout	 : NATURAL := 890;
  CONSTANT rup_b1_cout	 : NATURAL := 890;
  CONSTANT rup_b1_sout	 : NATURAL := 890;
  CONSTANT rup_b2_cout	 : NATURAL := 890;
  CONSTANT rup_b2_sout	 : NATURAL := 890;
  CONSTANT rup_b3_sout	 : NATURAL := 890;
  CONSTANT rup_b4_sout	 : NATURAL := 890;
  CONSTANT rup_cin1_cout	 : NATURAL := 890;
  CONSTANT rup_cin1_sout	 : NATURAL := 890;
  CONSTANT rup_cin2_sout	 : NATURAL := 890;
  CONSTANT rup_cin3_sout	 : NATURAL := 890;
  CONSTANT tphh_cin3_sout	 : NATURAL := 630;
  CONSTANT tphh_a4_sout	 : NATURAL := 673;
  CONSTANT tphh_b4_sout	 : NATURAL := 715;
  CONSTANT tphh_a1_cout	 : NATURAL := 800;
  CONSTANT tphh_a2_cout	 : NATURAL := 801;
  CONSTANT tpll_cin1_cout	 : NATURAL := 830;
  CONSTANT tpll_b1_cout	 : NATURAL := 839;
  CONSTANT tpll_a1_cout	 : NATURAL := 866;
  CONSTANT tpll_b2_cout	 : NATURAL := 883;
  CONSTANT tphh_b1_cout	 : NATURAL := 884;
  CONSTANT tphh_b2_cout	 : NATURAL := 892;
  CONSTANT tphh_cin1_cout	 : NATURAL := 899;
  CONSTANT tpll_a2_cout	 : NATURAL := 924;
  CONSTANT tphh_a3_sout	 : NATURAL := 1086;
  CONSTANT tpll_cin2_sout	 : NATURAL := 1150;
  CONSTANT tphh_b3_sout	 : NATURAL := 1202;
  CONSTANT tpll_b3_sout	 : NATURAL := 1208;
  CONSTANT tpll_a3_sout	 : NATURAL := 1265;
  CONSTANT tphh_cin2_sout	 : NATURAL := 1308;
  CONSTANT tpll_b4_sout	 : NATURAL := 1329;
  CONSTANT tpll_a4_sout	 : NATURAL := 1377;
  CONSTANT tpll_cin3_sout	 : NATURAL := 1417;
  CONSTANT tphl_a1_sout	 : NATURAL := 1471;
  CONSTANT tphl_a2_sout	 : NATURAL := 1472;
  CONSTANT tplh_cin1_sout	 : NATURAL := 1492;
  CONSTANT tplh_b1_sout	 : NATURAL := 1501;
  CONSTANT tplh_a1_sout	 : NATURAL := 1528;
  CONSTANT tplh_b2_sout	 : NATURAL := 1545;
  CONSTANT tphl_b1_sout	 : NATURAL := 1555;
  CONSTANT tphl_b2_sout	 : NATURAL := 1563;
  CONSTANT tphl_cin1_sout	 : NATURAL := 1570;
  CONSTANT tplh_a2_sout	 : NATURAL := 1586;
  CONSTANT transistors	 : NATURAL := 32
);
PORT (
  a1	 : in  BIT;
  a2	 : in  BIT;
  a3	 : in  BIT;
  a4	 : in  BIT;
  b1	 : in  BIT;
  b2	 : in  BIT;
  b3	 : in  BIT;
  b4	 : in  BIT;
  cin1	 : in  BIT;
  cin2	 : in  BIT;
  cin3	 : in  BIT;
  cout	 : out BIT;
  sout	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END fulladder_x4;

ARCHITECTURE behaviour_data_flow OF fulladder_x4 IS
  SIGNAL ncout	 : BIT;

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on fulladder_x4"
  SEVERITY WARNING;
  ASSERT (((((a1 and a2) and a3) and a4) or not ((((a1 or a2) or a3) or
  a4))) = '1')
  REPORT "a1, a2, a3, a4 must be connected together on fulladder_x4"
  SEVERITY WARNING;
  ASSERT (((((b1 and b2) and b3) and b4) or not ((((b1 or b2) or b3) or
  b4))) = '1')
  REPORT "b1, b2, b3, b4 must be connected together on fulladder_x4"
  SEVERITY WARNING;
  ASSERT ((((cin1 and cin2) and cin3) or not (((cin1 or cin2) or cin3))) = '1')
  REPORT "cin1, cin2, cin3 must be connected together on fulladder_x4"
  SEVERITY WARNING;
  ncout <= not (((a1 and b1) or ((a2 or b2) and cin1)));
  sout <= (((a3 and b3) and cin2) or (((a4 or b4) or cin3) and ncout)) after 2200 ps;
  cout <= not (ncout) after 1500 ps;
END;
