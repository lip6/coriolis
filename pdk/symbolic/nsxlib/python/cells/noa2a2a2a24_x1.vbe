ENTITY noa2a2a2a24_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 3500;
  CONSTANT cin_i0	 : NATURAL := 14;
  CONSTANT cin_i1	 : NATURAL := 14;
  CONSTANT cin_i2	 : NATURAL := 13;
  CONSTANT cin_i3	 : NATURAL := 13;
  CONSTANT cin_i4	 : NATURAL := 13;
  CONSTANT cin_i5	 : NATURAL := 13;
  CONSTANT cin_i6	 : NATURAL := 14;
  CONSTANT cin_i7	 : NATURAL := 14;
  CONSTANT rdown_i0_nq	 : NATURAL := 2850;
  CONSTANT rdown_i1_nq	 : NATURAL := 2850;
  CONSTANT rdown_i2_nq	 : NATURAL := 2850;
  CONSTANT rdown_i3_nq	 : NATURAL := 2850;
  CONSTANT rdown_i4_nq	 : NATURAL := 2850;
  CONSTANT rdown_i5_nq	 : NATURAL := 2850;
  CONSTANT rdown_i6_nq	 : NATURAL := 2850;
  CONSTANT rdown_i7_nq	 : NATURAL := 2850;
  CONSTANT rup_i0_nq	 : NATURAL := 6190;
  CONSTANT rup_i1_nq	 : NATURAL := 6190;
  CONSTANT rup_i2_nq	 : NATURAL := 6190;
  CONSTANT rup_i3_nq	 : NATURAL := 6190;
  CONSTANT rup_i4_nq	 : NATURAL := 6190;
  CONSTANT rup_i5_nq	 : NATURAL := 6190;
  CONSTANT rup_i6_nq	 : NATURAL := 6190;
  CONSTANT rup_i7_nq	 : NATURAL := 6190;
  CONSTANT tphl_i7_nq	 : NATURAL := 200;
  CONSTANT tphl_i6_nq	 : NATURAL := 270;
  CONSTANT tphl_i5_nq	 : NATURAL := 329;
  CONSTANT tphl_i4_nq	 : NATURAL := 419;
  CONSTANT tplh_i6_nq	 : NATURAL := 535;
  CONSTANT tphl_i2_nq	 : NATURAL := 550;
  CONSTANT tplh_i1_nq	 : NATURAL := 562;
  CONSTANT tplh_i7_nq	 : NATURAL := 591;
  CONSTANT tplh_i0_nq	 : NATURAL := 606;
  CONSTANT tplh_i4_nq	 : NATURAL := 613;
  CONSTANT tplh_i3_nq	 : NATURAL := 616;
  CONSTANT tphl_i0_nq	 : NATURAL := 649;
  CONSTANT tplh_i2_nq	 : NATURAL := 662;
  CONSTANT tplh_i5_nq	 : NATURAL := 662;
  CONSTANT tphl_i3_nq	 : NATURAL := 667;
  CONSTANT tphl_i1_nq	 : NATURAL := 775;
  CONSTANT transistors	 : NATURAL := 16
);
PORT (
  i0	 : in  BIT;
  i1	 : in  BIT;
  i2	 : in  BIT;
  i3	 : in  BIT;
  i4	 : in  BIT;
  i5	 : in  BIT;
  i6	 : in  BIT;
  i7	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END noa2a2a2a24_x1;

ARCHITECTURE behaviour_data_flow OF noa2a2a2a24_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on noa2a2a2a24_x1"
  SEVERITY WARNING;
  nq <= not (((((i0 and i1) or (i2 and i3)) or (i4 and i5)) or (i6 and
  i7))) after 1400 ps;
END;
