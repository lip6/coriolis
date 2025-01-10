ENTITY fulladder_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 5000;
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
  CONSTANT rdown_a1_cout	 : NATURAL := 1620;
  CONSTANT rdown_a1_sout	 : NATURAL := 1620;
  CONSTANT rdown_a2_cout	 : NATURAL := 1620;
  CONSTANT rdown_a2_sout	 : NATURAL := 1620;
  CONSTANT rdown_a3_sout	 : NATURAL := 1620;
  CONSTANT rdown_a4_sout	 : NATURAL := 1620;
  CONSTANT rdown_b1_cout	 : NATURAL := 1620;
  CONSTANT rdown_b1_sout	 : NATURAL := 1620;
  CONSTANT rdown_b2_cout	 : NATURAL := 1620;
  CONSTANT rdown_b2_sout	 : NATURAL := 1620;
  CONSTANT rdown_b3_sout	 : NATURAL := 1620;
  CONSTANT rdown_b4_sout	 : NATURAL := 1620;
  CONSTANT rdown_cin1_cout	 : NATURAL := 1620;
  CONSTANT rdown_cin1_sout	 : NATURAL := 1620;
  CONSTANT rdown_cin2_sout	 : NATURAL := 1620;
  CONSTANT rdown_cin3_sout	 : NATURAL := 1620;
  CONSTANT rup_a1_cout	 : NATURAL := 1790;
  CONSTANT rup_a1_sout	 : NATURAL := 1790;
  CONSTANT rup_a2_cout	 : NATURAL := 1790;
  CONSTANT rup_a2_sout	 : NATURAL := 1790;
  CONSTANT rup_a3_sout	 : NATURAL := 1790;
  CONSTANT rup_a4_sout	 : NATURAL := 1790;
  CONSTANT rup_b1_cout	 : NATURAL := 1790;
  CONSTANT rup_b1_sout	 : NATURAL := 1790;
  CONSTANT rup_b2_cout	 : NATURAL := 1790;
  CONSTANT rup_b2_sout	 : NATURAL := 1790;
  CONSTANT rup_b3_sout	 : NATURAL := 1790;
  CONSTANT rup_b4_sout	 : NATURAL := 1790;
  CONSTANT rup_cin1_cout	 : NATURAL := 1790;
  CONSTANT rup_cin1_sout	 : NATURAL := 1790;
  CONSTANT rup_cin2_sout	 : NATURAL := 1790;
  CONSTANT rup_cin3_sout	 : NATURAL := 1790;
  CONSTANT tphh_cin3_sout	 : NATURAL := 489;
  CONSTANT tphh_a4_sout	 : NATURAL := 536;
  CONSTANT tphh_b4_sout	 : NATURAL := 581;
  CONSTANT tphh_a2_cout	 : NATURAL := 658;
  CONSTANT tpll_cin1_cout	 : NATURAL := 694;
  CONSTANT tphh_a1_cout	 : NATURAL := 699;
  CONSTANT tpll_b1_cout	 : NATURAL := 709;
  CONSTANT tpll_a1_cout	 : NATURAL := 736;
  CONSTANT tphh_cin1_cout	 : NATURAL := 742;
  CONSTANT tpll_b2_cout	 : NATURAL := 748;
  CONSTANT tphh_b2_cout	 : NATURAL := 751;
  CONSTANT tphh_b1_cout	 : NATURAL := 777;
  CONSTANT tpll_a2_cout	 : NATURAL := 782;
  CONSTANT tpll_cin2_sout	 : NATURAL := 893;
  CONSTANT tphh_a3_sout	 : NATURAL := 902;
  CONSTANT tpll_b3_sout	 : NATURAL := 951;
  CONSTANT tpll_a3_sout	 : NATURAL := 1008;
  CONSTANT tphh_b3_sout	 : NATURAL := 1014;
  CONSTANT tpll_b4_sout	 : NATURAL := 1071;
  CONSTANT tpll_a4_sout	 : NATURAL := 1114;
  CONSTANT tphh_cin2_sout	 : NATURAL := 1116;
  CONSTANT tphl_a2_sout	 : NATURAL := 1128;
  CONSTANT tpll_cin3_sout	 : NATURAL := 1149;
  CONSTANT tplh_cin1_sout	 : NATURAL := 1163;
  CONSTANT tphl_a1_sout	 : NATURAL := 1169;
  CONSTANT tplh_b1_sout	 : NATURAL := 1178;
  CONSTANT tplh_a1_sout	 : NATURAL := 1205;
  CONSTANT tphl_cin1_sout	 : NATURAL := 1212;
  CONSTANT tplh_b2_sout	 : NATURAL := 1217;
  CONSTANT tphl_b2_sout	 : NATURAL := 1221;
  CONSTANT tphl_b1_sout	 : NATURAL := 1247;
  CONSTANT tplh_a2_sout	 : NATURAL := 1251;
  CONSTANT transistors	 : NATURAL := 28
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
END fulladder_x2;

ARCHITECTURE behaviour_data_flow OF fulladder_x2 IS
  SIGNAL ncout	 : BIT;

BEGIN
  ASSERT ((((cin1 and cin2) and cin3) or not (((cin1 or cin2) or cin3))) = '1')
  REPORT "cin1, cin2, cin3 must be connected together on fulladder_x2"
  SEVERITY WARNING;
  ASSERT (((((b1 and b2) and b3) and b4) or not ((((b1 or b2) or b3) or
  b4))) = '1')
  REPORT "b1, b2, b3, b4 must be connected together on fulladder_x2"
  SEVERITY WARNING;
  ASSERT (((((a1 and a2) and a3) and a4) or not ((((a1 or a2) or a3) or
  a4))) = '1')
  REPORT "a1, a2, a3, a4 must be connected together on fulladder_x2"
  SEVERITY WARNING;
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on fulladder_x2"
  SEVERITY WARNING;
  ncout <= not (((a1 and b1) or ((a2 or b2) and cin1)));
  sout <= (((a3 and b3) and cin2) or (((a4 or b4) or cin3) and ncout)) after 1900 ps;
  cout <= not (ncout) after 1400 ps;
END;
