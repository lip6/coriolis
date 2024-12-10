ENTITY nao2o22_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 2750;
  CONSTANT cin_i0	 : NATURAL := 8;
  CONSTANT cin_i1	 : NATURAL := 8;
  CONSTANT cin_i2	 : NATURAL := 8;
  CONSTANT cin_i3	 : NATURAL := 8;
  CONSTANT rdown_i0_nq	 : NATURAL := 810;
  CONSTANT rdown_i1_nq	 : NATURAL := 810;
  CONSTANT rdown_i2_nq	 : NATURAL := 810;
  CONSTANT rdown_i3_nq	 : NATURAL := 810;
  CONSTANT rup_i0_nq	 : NATURAL := 890;
  CONSTANT rup_i1_nq	 : NATURAL := 890;
  CONSTANT rup_i2_nq	 : NATURAL := 890;
  CONSTANT rup_i3_nq	 : NATURAL := 890;
  CONSTANT tphl_i3_nq	 : NATURAL := 607;
  CONSTANT tplh_i0_nq	 : NATURAL := 644;
  CONSTANT tphl_i2_nq	 : NATURAL := 664;
  CONSTANT tphl_i1_nq	 : NATURAL := 666;
  CONSTANT tplh_i1_nq	 : NATURAL := 717;
  CONSTANT tplh_i2_nq	 : NATURAL := 721;
  CONSTANT tphl_i0_nq	 : NATURAL := 734;
  CONSTANT tplh_i3_nq	 : NATURAL := 807;
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
END nao2o22_x4;

ARCHITECTURE behaviour_data_flow OF nao2o22_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on nao2o22_x4"
  SEVERITY WARNING;
  nq <= not (((i0 or i1) and (i2 or i3))) after 1400 ps;
END;
