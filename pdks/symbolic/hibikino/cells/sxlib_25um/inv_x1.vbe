ENTITY inv_x1 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 750;
  CONSTANT cin_i	 : NATURAL := 8;
  CONSTANT rdown_i_nq	 : NATURAL := 3640;
  CONSTANT rup_i_nq	 : NATURAL := 3720;
  CONSTANT tphl_i_nq	 : NATURAL := 101;
  CONSTANT tplh_i_nq	 : NATURAL := 139;
  CONSTANT transistors	 : NATURAL := 2
);
PORT (
  i	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END inv_x1;

ARCHITECTURE behaviour_data_flow OF inv_x1 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on inv_x1"
  SEVERITY WARNING;
  nq <= not (i) after 700 ps;
END;
