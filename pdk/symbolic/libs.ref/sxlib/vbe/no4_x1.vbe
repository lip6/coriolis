ENTITY no4_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1500;
  CONSTANT cin_i0	 : NATURAL := 12;
  CONSTANT cin_i1	 : NATURAL := 12;
  CONSTANT cin_i2	 : NATURAL := 12;
  CONSTANT cin_i3	 : NATURAL := 12;
  CONSTANT rdown_i0_nq	 : NATURAL := 3640;
  CONSTANT rdown_i1_nq	 : NATURAL := 3640;
  CONSTANT rdown_i2_nq	 : NATURAL := 3640;
  CONSTANT rdown_i3_nq	 : NATURAL := 3640;
  CONSTANT rup_i0_nq	 : NATURAL := 6190;
  CONSTANT rup_i1_nq	 : NATURAL := 6190;
  CONSTANT rup_i2_nq	 : NATURAL := 6190;
  CONSTANT rup_i3_nq	 : NATURAL := 6190;
  CONSTANT tphl_i1_nq	 : NATURAL := 230;
  CONSTANT tplh_i3_nq	 : NATURAL := 271;
  CONSTANT tplh_i1_nq	 : NATURAL := 320;
  CONSTANT tphl_i0_nq	 : NATURAL := 330;
  CONSTANT tplh_i2_nq	 : NATURAL := 333;
  CONSTANT tplh_i0_nq	 : NATURAL := 340;
  CONSTANT tphl_i2_nq	 : NATURAL := 419;
  CONSTANT tphl_i3_nq	 : NATURAL := 499;
  CONSTANT transistors	 : NATURAL := 8
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
END no4_x1;

ARCHITECTURE behaviour_data_flow OF no4_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on no4_x1"
  SEVERITY WARNING;
  nq <= not ((((i0 or i1) or i2) or i3)) after 1100 ps;
END;
