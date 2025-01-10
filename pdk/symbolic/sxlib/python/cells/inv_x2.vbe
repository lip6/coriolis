ENTITY inv_x2 IS
GENERIC (
  CONSTANT area 	 : NATURAL := 750;
  CONSTANT cin_i	 : NATURAL := 12;
  CONSTANT rdown_i_nq	 : NATURAL := 1620;
  CONSTANT rup_i_nq	 : NATURAL := 2420;
  CONSTANT tphl_i_nq	 : NATURAL := 69;
  CONSTANT tplh_i_nq	 : NATURAL := 163;
  CONSTANT transistors	 : NATURAL := 2
);
PORT (
  i	 : in  BIT;
  nq	 : out BIT;
  vdd	 : in  BIT;
  vss	 : in  BIT
);
END inv_x2;

ARCHITECTURE behaviour_data_flow OF inv_x2 IS

BEGIN
  ASSERT ((vdd and not (vss)) = '1')
  REPORT "power supply is missing on inv_x2"
  SEVERITY WARNING;
  nq <= not (i) after 800 ps;
END;
