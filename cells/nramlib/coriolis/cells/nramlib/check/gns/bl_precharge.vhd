--
-- Template bl_precharge
-- Recognition rule
--
entity bl_precharge is
    port    ( q, nq                : inout bit;
              ck                   : in bit;
              vdd, vss             : in bit);
    -- pragma symmetric q nq
end;

architecture structural of bl_precharge is
	component tn
        port    ( gate             : in    bit;
                  source, drain    : inout bit;
                  bulk             : in    bit);
    end component;

begin
    tn_1    : tn port map (ck ,q  ,vdd   ,vss);
    tn_2    : tn port map (ck ,nq ,vdd   ,vss);
    tn_3    : tn port map (ck ,q  ,nq    ,vss);

end;
