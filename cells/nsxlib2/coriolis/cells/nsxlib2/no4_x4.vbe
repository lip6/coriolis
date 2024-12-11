ENTITY no4_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2500;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 12;
  CONSTANT cin_i2	 : NATURAL := 12;
  CONSTANT cin_i3	 : NATURAL := 12;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rdown_i3_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT rup_i3_nq	 : NATURAL := 890;
  CONSTANT tphl_i1_nq	 : NATURAL := 564;
  CONSTANT tphl_i0_nq	 : NATURAL := 656;
  CONSTANT tplh_i3_nq	 : NATURAL := 693;
  CONSTANT tphl_i2_nq	 : NATURAL := 739;
  CONSTANT tplh_i2_nq	 : NATURAL := 761;
  CONSTANT tplh_i1_nq	 : NATURAL := 768;
  CONSTANT tplh_i0_nq	 : NATURAL := 777;
  CONSTANT tphl_i3_nq	 : NATURAL := 816;
  CONSTANT transistors	 : NATURAL := 14
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END no4_x4;

ARCHITECTURE behaviour_data_flow OF no4_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on no4_x4"
  SEVERITY WARNING;
  nq <= not ((((i0 or i1) or i2) or i3)) after 1400 ps;
END;
