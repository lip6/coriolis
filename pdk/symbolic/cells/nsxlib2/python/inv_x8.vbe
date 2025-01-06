ENTITY inv_x8 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1750;
  CONSTANT cin_i	 : NATURAL := 54;
  CONSTANT rdown_i_nq	 : NATURAL := 400;
  CONSTANT rup_i_nq	 : NATURAL := 450;
  CONSTANT tphl_i_nq	 : NATURAL := 86;
  CONSTANT tplh_i_nq	 : NATURAL := 133;
  CONSTANT transistors	 : NATURAL := 8
);
PORT (
  i	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END inv_x8;

ARCHITECTURE behaviour_data_flow OF inv_x8 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on inv_x8"
  SEVERITY WARNING;
  nq <= not (i) after 700 ps;
END;
