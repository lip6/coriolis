ENTITY inv_x4 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 1000;
  CONSTANT cin_i	 : NATURAL := 26;
  CONSTANT rdown_i_nq	 : NATURAL := 810;
  CONSTANT rup_i_nq	 : NATURAL := 1060;
  CONSTANT tphl_i_nq	 : NATURAL := 71;
  CONSTANT tplh_i_nq	 : NATURAL := 143;
  CONSTANT transistors	 : NATURAL := 4
);
PORT (
  i	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END inv_x4;

ARCHITECTURE behaviour_data_flow OF inv_x4 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on inv_x4"
  SEVERITY WARNING;
  nq <= not (i) after 700 ps;
END;
